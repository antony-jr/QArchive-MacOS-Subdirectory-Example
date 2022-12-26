#include <QCoreApplication>
#include <QArchive>

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);

    QArchive::DiskCompressor dcomp("test.zip");
    QArchive::MemoryCompressor compressor(QArchive::SevenZipFormat);
    QArchive::MemoryExtractor extractor;

    /* Write the file to compress and add it. */
    QString contents = "Hello World from QArchive!";
    auto array = contents.toLocal8Bit();
    QBuffer buffer(&array);
    dcomp.addFiles("readme-fin.txt", &buffer);
    compressor.addFiles("readme.txt", &buffer);

    QObject::connect(&extractor, &QArchive::MemoryExtractor::error, [&]() {
        app.exit(-1);
    });

    QObject::connect(&dcomp, &QArchive::DiskCompressor::error, [&]() { app.exit(-1); });

    QObject::connect(&compressor, &QArchive::MemoryCompressor::error, [&]() {
        app.exit(-1);
    });

    QObject::connect(&dcomp, &QArchive::DiskCompressor::finished, [&]() {
	extractor.setArchive("test.zip");
	extractor.start();
    });

    QObject::connect(&compressor, &QArchive::MemoryCompressor::finished, [&](QBuffer *archive) {
        extractor.setArchive(archive);
        extractor.start();
    });

    QObject::connect(&extractor, &QArchive::MemoryExtractor::finished, [&](QArchive::MemoryExtractorOutput *output) {
        auto files = output->getFiles();
        QString fileName;
	for(auto iter = files.begin(),
                end = files.end();
                iter != end;
                ++iter) {
            auto fileInfo = (*iter).fileInformation();
            auto buffer = (*iter).buffer();
            
	    fileName = fileInfo.value("FileName").toString();
	    qDebug() << "FileName:: " << fileInfo.value("FileName").toString();

            buffer->open(QIODevice::ReadOnly);
            qDebug() << "Contents:: " << QString(buffer->readAll());
            buffer->close();
        }
        output->deleteLater();

	if (fileName == "readme-fin.txt") {
           app.quit();
	} else {
	   dcomp.start();
	}	   
    });

    compressor.start();

    return app.exec();
}
