#include "studydesk/core.hpp"
#include <QApplication>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <iostream>
#include <stdexcept>

namespace {
class QtFrontend final : public studydesk::IFrontend {
public:
    explicit QtFrontend(QApplication& app) : app_(app) {}
    int run(studydesk::ITaskService& service) override {
        QWidget window;
        window.setWindowTitle("StudyDesk | Interface Lab");
        window.resize(520, 380);
        auto* layout = new QVBoxLayout(&window);
        auto* heading = new QLabel("StudyDesk - one service, several interfaces");
        auto* input = new QLineEdit;
        input->setPlaceholderText("A task to study (up to 80 UTF-8 bytes)");
        auto* add = new QPushButton("Add task");
        auto* list = new QListWidget;
        auto* status = new QLabel("Tasks are in memory for this session.");
        layout->addWidget(heading);
        layout->addWidget(input);
        layout->addWidget(add);
        layout->addWidget(list);
        layout->addWidget(status);
        auto refresh = [&] {
            list->clear();
            for (const auto& task : service.tasks()) {
                list->addItem(QString::fromStdString(task.title));
            }
        };
        auto addTask = [&] {
            try {
                service.addTask(input->text().toStdString());
                input->clear();
                refresh();
                status->setText("Task added.");
            } catch (const std::exception& error) {
                status->setText(QString::fromUtf8(error.what()));
            }
        };
        QObject::connect(add, &QPushButton::clicked, &window, addTask);
        QObject::connect(input, &QLineEdit::returnPressed, &window, addTask);
        refresh();
        window.show();
        // CI invokes actual widgets and their callbacks without a display.
        if (app_.arguments().contains("--smoke-test")) {
            QTimer::singleShot(0, &window, [&] {
                input->setText("GUI smoke task");
                add->click();
                const bool added = service.tasks().size() == 1 && list->count() == 1;
                input->setText("   ");
                add->click();
                const bool rejected = service.tasks().size() == 1;
                app_.exit(added && rejected ? 0 : 1);
            });
        }
        return app_.exec();
    }

private:
    QApplication& app_;
};
} // namespace
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    studydesk::MemoryTaskStore store;
    studydesk::StreamLogger logger(std::cout);
    studydesk::TaskService service(store, logger);
    QtFrontend gui(app);
    studydesk::IFrontend& frontend = gui;
    return frontend.run(service);
}
