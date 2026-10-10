"""Exercise the real C++ HTTP server using only Python's standard library."""
import http.client
import queue
import re
import subprocess
import sys
import threading
import urllib.parse

process = subprocess.Popen([sys.argv[1], "0"], stdout=subprocess.PIPE,
                           stderr=subprocess.STDOUT, text=True)
lines = queue.Queue()
def read_output():
    for line in process.stdout:
        lines.put(line)
threading.Thread(target=read_output, daemon=True).start()
try:
    line = lines.get(timeout=15)
    match = re.search(r"127\.0\.0\.1:(\d+)", line)
    if not match:
        raise RuntimeError("Server did not report a listening port: " + line)
    port = int(match.group(1))
    def request(method, path, body=None):
        connection = http.client.HTTPConnection("127.0.0.1", port, timeout=5)
        headers = {"Content-Type": "application/x-www-form-urlencoded"} if body else {}
        connection.request(method, path, body, headers)
        response = connection.getresponse()
        result = response.status, response.read().decode("utf-8"), {key.lower(): value for key, value in response.getheaders()}
        connection.close()
        return result
    def require(condition, message):
        if not condition:
            raise RuntimeError(message)
    require(request("GET", "/")[0] == 200, "Home page failed")
    title = 'Read <C++> & "interfaces"'
    status, _, headers = request("POST", "/add", urllib.parse.urlencode({"title": title}))
    require(status == 303 and headers.get("location") == "/", "Valid POST must redirect")
    _, html, _ = request("GET", "/")
    require("Read &lt;C++&gt; &amp; &quot;interfaces&quot;" in html, "HTML must escape the task")
    require(title not in html, "Raw user text must not become markup")
    status, _, _ = request("POST", "/add", urllib.parse.urlencode({"title": "   "}))
    require(status == 400, "Blank task must be rejected by the shared service")
    status, _, _ = request("POST", "/add", urllib.parse.urlencode({"title": "x" * 81}))
    require(status == 400, "Long task must be rejected by the shared service")
    require(request("GET", "/missing")[0] == 404, "Missing route must return 404")
    print("HTTP add/list, encoding, validation and missing route checks passed.")
finally:
    process.terminate()
    try:
        process.wait(timeout=5)
    except subprocess.TimeoutExpired:
        process.kill()
        process.wait(timeout=5)
