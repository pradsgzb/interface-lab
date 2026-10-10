#include "studydesk/core.hpp"
#include <QCoreApplication>
#include <QHostAddress>
#include <QHttpServer>
#include <QHttpServerRequest>
#include <QHttpServerResponse>
#include <QTcpServer>
#include <QUrlQuery>
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
#include <QHttpHeaders>
#endif
#include <iostream>
#include <stdexcept>

namespace {
// Qt handles HTTP parsing. This file teaches an adapter, not socket programming.
QString page(const studydesk::ITaskService& service, const QString& message = {}) {
    QString items;
    for (const auto& task : service.tasks()) {
        items += "<li>" + QString::fromStdString(task.title).toHtmlEscaped() + "</li>";
    }
    if (items.isEmpty()) {
        items = "<li>No tasks yet.</li>";
    }
    return QString::fromUtf8(R"HTML(<!doctype html>
<html lang="en"><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>StudyDesk | Interface Lab</title><style>
body{font:18px system-ui,sans-serif;max-width:720px;margin:64px auto;padding:0 24px;background:#f2f5fa;color:#13213a}
main{padding:32px;background:white;border-radius:16px}h1{margin-top:0}input,button{font:inherit;padding:12px;box-sizing:border-box}
input{width:100%;margin-bottom:12px}button{background:#174cc5;color:white;border:0;border-radius:6px;cursor:pointer}
li{padding:8px}small{color:#53617b}.message{font-weight:600}
</style></head><body><main><small>C++ INTERFACE LAB / WEB FRONTEND</small><h1>StudyDesk</h1>
<p>The same C++ task service powers the console, desktop and web frontends.</p>
<form action="/add" method="post"><label for="title">New study task</label>
<input id="title" name="title" maxlength="80" required placeholder="Revise C++ interfaces">
<button type="submit">Add task</button></form><p class="message">%1</p><ul>%2</ul>
<small>Local teaching demo. In-memory tasks reset when this process exits.</small></main></body></html>)HTML")
        .arg(message.toHtmlEscaped(), items);
}
class WebFrontend final : public studydesk::IFrontend {
public:
    WebFrontend(QCoreApplication& app, quint16 port) : app_(app), port_(port) {}
    int run(studydesk::ITaskService& service) override {
        QHttpServer server;
        server.route("/", QHttpServerRequest::Method::Get, [&service] {
            return QHttpServerResponse("text/html; charset=utf-8", page(service).toUtf8());
        });
        server.route(
            "/add", QHttpServerRequest::Method::Post,
            [&service](const QHttpServerRequest& request) {
                // A normal HTML form encodes spaces as '+'. Decode through Qt.
                const QByteArray form = request.body();
                if (form.size() > 4096) {
                    return QHttpServerResponse(QHttpServerResponder::StatusCode::BadRequest);
                }
                const QString encoded = QString::fromUtf8(form).replace('+', "%20");
                const QUrlQuery values(encoded);
                const auto title = values.queryItemValue("title", QUrl::FullyDecoded);
                try {
                    service.addTask(title.toStdString());
                    QHttpServerResponse response(QHttpServerResponder::StatusCode::SeeOther);
                // Refresh will not resubmit the POST. Qt 6.8 changed header APIs.
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
                    QHttpHeaders headers;
                    headers.append(QHttpHeaders::WellKnownHeader::Location, "/");
                    response.setHeaders(std::move(headers));
#else
                response.setHeader("Location", "/");
#endif
                    return response;
                } catch (const std::invalid_argument& error) {
                    return QHttpServerResponse(
                        "text/html; charset=utf-8",
                        page(service, QString::fromUtf8(error.what())).toUtf8(),
                        QHttpServerResponder::StatusCode::BadRequest);
                }
            });
        QTcpServer tcp;
        // Loopback only: this is a local lesson, not a public hosting server.
        if (!tcp.listen(QHostAddress::LocalHost, port_)) {
            std::cerr << "Cannot listen on 127.0.0.1:" << port_ << '\n';
            return 1;
        }
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
        if (!server.bind(&tcp)) {
            return 1;
        }
#else
        server.bind(&tcp); // Qt 6.4-6.7 returns void; 6.8+ returns bool.
#endif
        std::cout << "Open http://127.0.0.1:" << tcp.serverPort() << "/ (Ctrl+C to stop)"
                  << std::endl;
        return app_.exec();
    }

private:
    QCoreApplication& app_;
    quint16 port_;
};
} // namespace
int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    quint16 port = 8080;
    if (argc > 1) {
        bool valid = false;
        const uint value = QString::fromLocal8Bit(argv[1]).toUInt(&valid);
        if (!valid || value > 65535 || argc > 2) {
            std::cerr << "Usage: studydesk_web [port: 0-65535]\n";
            return 2;
        }
        port = static_cast<quint16>(value); // 0 requests an available local port.
    }
    studydesk::MemoryTaskStore store;
    studydesk::StreamLogger logger(std::cout);
    studydesk::TaskService service(store, logger);
    WebFrontend web(app, port);
    studydesk::IFrontend& frontend = web;
    return frontend.run(service);
}
