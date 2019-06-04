from PySide2.QtGui import QGuiApplication
from PySide2.QtQml import QQmlApplicationEngine
from PySide2.QtCore import QUrl, qInstallMessageHandler, QtInfoMsg, QtWarningMsg, QtCriticalMsg, QtFatalMsg
import os

def msg_handler(mode, context, message):
    if mode == QtInfoMsg:
        mode = 'info'
    elif mode == QtWarningMsg:
        mode = 'warn'
    elif mode == QtCriticalMsg:
        mode = 'critical'
    elif mode == QtFatalMsg:
        mode = 'fatal'
    else:
        mode = 'Debug'
    print("%s: %s (%s:%d, %s)" % (mode, message, context.file, context.line, context.file))

def run(args, field):
    qInstallMessageHandler(msg_handler)

    if "QT_QUICK_CONTROLS_STYLE" not in os.environ:
        os.environ["QT_QUICK_CONTROLS_STYLE"] = "Fusion"

    app = QGuiApplication(args)
    eng = QQmlApplicationEngine()
    eng.rootContext().setContextProperty("field", field)
    eng.load(QUrl("ui.qml"))
    app.exec_()