#include <QCommandLineParser>
#include <QDir>
#include <QGuiApplication>
#include <QLocale>
#include <QQmlApplicationEngine>
#include <QTemporaryFile>
#include <QThread>
#include <QTranslator>

#include <g3log/logcapture.hpp>
#include <g3log/logworker.hpp>
#include <g3logwrapper/log.h>

#include <amsterqt/facade.h>
using namespace amster::qt;

// log capture
void qtMsgToLogHandler(QtMsgType _type, const QMessageLogContext& _context, const QString& _msg)
{
    const char* _function = _context.function ? _context.function : "";
    const char* _file = _context.file ? _context.file : "";
    const QByteArray& _theMsg = _msg.toLocal8Bit();
    LEVELS _level{
        _type == QtInfoMsg
            ? INFO
            : _type == QtDebugMsg
                ? DBUG
                : _type == QtWarningMsg
                    ? WARNING
                    : FATAL
    };
    LogCapture(_file, _context.line, _function, _level).stream() << _theMsg.data();
}

struct BoostOptions {
    bool standalone = { false };
    bool distribute = { false };
    bool master = { false };
    uint32_t domain = { 0 };
    QString configure_file;
};

enum CommandLineParseResult {
    CommandLineOk,
    CommandLineError,
    CommandLineVersionRequested,
    CommandLineHelpRequested
};

CommandLineParseResult parseCommandLine(QCommandLineParser& parser, BoostOptions* _options, QString* errorMessage)
{
    parser.addPositionalArgument(
        "configure file",
        QCoreApplication::translate("main",
            "use defined configure file path."));
    parser.addOptions({ // A boolean option with a single name (-c)
        { "S",
            QCoreApplication::translate("main", "Run as a Single-Progress StandAlone AMSTER App.") },
        { "D",
            QCoreApplication::translate("main", "Single-Progress with Distribute mode.") },
        { { "m", "master" },
            QCoreApplication::translate("main", "Play a Master in distribute mode.") },
        { { "d", "domain" },
            QCoreApplication::translate("main", "The Domain Value."),
            QCoreApplication::translate("main", "Default is 0 or 1 when in distribute mode.") },
        { { "f", "configure_file" },
            QCoreApplication::translate("main", "Use another configure file."),
            QCoreApplication::translate("main", "the file path") } });

    const QCommandLineOption helpOption = parser.addHelpOption();
    const QCommandLineOption versionOption = parser.addVersionOption();

    if (!parser.parse(QCoreApplication::arguments())) {
        *errorMessage = parser.errorText();
        return CommandLineError;
    }

    const QStringList positionalArguments = parser.positionalArguments();

    if (positionalArguments.size() > 1) {
        *errorMessage = "Several 'name' arguments specified.";
        return CommandLineError;
    }
    if (positionalArguments.size() == 1)
        _options->configure_file = positionalArguments.first();

    if (parser.isSet("S")) {
        _options->standalone = true;
    }
    if (parser.isSet("D")) {
        _options->distribute = true;
        _options->master = parser.isSet("m");
    }

    _options->domain = parser.isSet("d") ? parser.value("d").toUInt()
                                         : _options->distribute ? 1:0;

    if (parser.isSet(versionOption))
        return CommandLineVersionRequested;

    if (parser.isSet(helpOption))
        return CommandLineHelpRequested;

    return CommandLineOk;
}
int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    auto _worker = g3logwrapper::Log::initLogging();
    {
        g3logwrapper::Log::initConsoleSink(_worker.get(), { DBUG });
#ifndef Q_OS_ANDROID
        QString _logFile = QString(QLatin1String("./logs/gamelifesim"));
        g3logwrapper::Log::initRotateFileSink(_worker.get(), { DBUG }, _logFile.toStdString(), 5, 5 * 1024 * 1024);
#endif
    }

    QGuiApplication app(argc, argv);
    app.setOrganizationName("PhoneYou");
    app.setOrganizationDomain("gamelife.phoneyou.net");
    app.setApplicationName("GAMELIFE Sim");
    app.setApplicationVersion("1.0.0");

//    QTranslator appTranslator;
//    appTranslator.load(QLocale(), "gamelifesim", "_", ":/i18n/");
//    QCoreApplication::installTranslator(&appTranslator);

    QCommandLineParser parser;
    parser.setApplicationDescription("GAMELIFE copyright by PhoneYou Inc.");
    BoostOptions options;
    QString errorMessage;
    switch (parseCommandLine(parser, &options, &errorMessage)) {
    case CommandLineOk:
        break;
    case CommandLineError:
        fputs(qPrintable(errorMessage), stderr);
        fputs("\n\n", stderr);
        fputs(qPrintable(parser.helpText()), stderr);
        return 1;
    case CommandLineVersionRequested:
        printf("%s %s\n", qPrintable(QCoreApplication::applicationName()),
            qPrintable(QCoreApplication::applicationVersion()));
        return 0;
    case CommandLineHelpRequested:
        parser.showHelp();
        Q_UNREACHABLE();
    }

    QTemporaryFile _configFile;
    {
        _configFile.open();
        QFile _fileI{ !options.configure_file.isEmpty()
                ? options.configure_file
                : options.standalone
                    ? ":/config/standalone.json"
                    : options.distribute
                        ? (options.master
                                  ? ":/config/standalone_master.json"
                                  : ":/config/standalone_slaver.json")
                        : ":/config/amster.json" };
        _fileI.open(QIODevice::ReadOnly);
        _configFile.write(_fileI.readAll());
        _configFile.flush();
    }
    Facade::instance()->init(_configFile.fileName(),options.domain);
    Facade::instance()->startAsync();

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    QDir _amsterImportPath(qgetenv("AMSTERFWK"));
    if (_amsterImportPath.cd("qml"))
        engine.addImportPath(_amsterImportPath.path());
    engine.load(url);
    QObject::connect(&app, &QCoreApplication::aboutToQuit, [] {
        Facade::instance()->destroy();
    });

    return app.exec();
}
