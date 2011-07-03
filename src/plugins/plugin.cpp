#include "plugin.h"
#include "plugininterface.h"
#include "pluginmanager.h"

#ifdef Q_WS_WIN
    #include <windows.h>
#endif

#include <QDir>
#include <QFileInfo>
#include <QLibrary>

#include <QMessageBox>

namespace OpenCOR {

Plugin::Plugin(const QString &pName) :
    mName(pName),
    mStatus(Undefined)
{
}

Plugin::~Plugin()
{
}

QString Plugin::name()
{
    // Return the plugin's

    return mName;
}

Plugin::PluginStatus Plugin::status()
{
    // Return the plugin's status

    return mStatus;
}

void Plugin::setStatus(const PluginStatus &pStatus)
{
    // Set the plugin's new status

    mStatus = pStatus;
}

QString Plugin::name(const QString &pPluginFileName)
{
    // Return the plugin's name based on its file name

    return QFileInfo(pPluginFileName).baseName().remove(0, PluginPrefix.length());
    // Note: we must remove the plugin prefix part from the plugin file name...
}

QString Plugin::fileName(const QString &pPluginsDir, const QString &pPluginName)
{
    // Return the plugin's file name based on its name

    return pPluginsDir+QDir::separator()+PluginPrefix+pPluginName+PluginExtension;
    // Note: we must add the plugin prefix part to the plugin file name...
}

#ifdef Q_WS_WIN
QStringList Plugin::dependencies(const QString &pPluginFileName)
{
    // Return the plugin's dependencies
    // Note: the following is heavily based on the code which was found at
    //       http://pastebin.com/XNMtZF8G

    QStringList dependencies = QStringList();

    // Get a file mapping for the plugin

    void *fileMapping = 0;

    HANDLE hFile = CreateFileA(pPluginFileName.toLatin1().constData(),
                               GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE,
                               0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);

    if (hFile != INVALID_HANDLE_VALUE) {
        HANDLE hMapping = CreateFileMapping(hFile, 0, PAGE_READONLY|SEC_COMMIT,
                                            0, 0, 0);

        if (hMapping) {
            fileMapping = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);

            CloseHandle(hMapping);
        }

        CloseHandle(hFile);
    }

    // Make sure that we have a file mapping for the plugin

    if (!fileMapping)
        return dependencies;

    // Retrieve the plugin's DOS header

    IMAGE_DOS_HEADER *dosHeader = (IMAGE_DOS_HEADER *) fileMapping;

    struct Header
    {
            DWORD signature;
            IMAGE_FILE_HEADER fileHeader;
            IMAGE_OPTIONAL_HEADER optionalHeader;
            IMAGE_SECTION_HEADER sectionHeader[];
    } *header = 0;

    if (!IsBadReadPtr(dosHeader, sizeof(*dosHeader)))
        if (dosHeader->e_magic == IMAGE_DOS_SIGNATURE)
            header = (Header *) ((char *) dosHeader+dosHeader->e_lfanew);

    // Make sure that the plugin's DOS header was retrieved

    if (!header) {
        UnmapViewOfFile(fileMapping);

        return dependencies;
    }

    // Make sure we can read the plugin's DOS header

    if (IsBadReadPtr(header, sizeof(*header))) {
        UnmapViewOfFile(fileMapping);

        return dependencies;
    }

    // Make sure that the DOS header has the right signature

    if (header->signature != IMAGE_NT_SIGNATURE) {
        UnmapViewOfFile(fileMapping);

        return dependencies;
    }

    // Retrieve the number of sections in the DOS header

    int nbOfSections = header->fileHeader.NumberOfSections;

    // Make sure that sections were found

    if (!nbOfSections) {
        UnmapViewOfFile(fileMapping);

        return dependencies;
    }

    // Make sure that the sections can be read

    if (IsBadReadPtr(header->sectionHeader,
                     nbOfSections*sizeof(IMAGE_SECTION_HEADER))) {
        UnmapViewOfFile(fileMapping);

        return dependencies;
    }

    // Retrieve the sections' data

    struct SectionData
    {
        int length;
        char *data;
    } *sectionData = new SectionData[nbOfSections];

    for (int section = 0; section < nbOfSections; ++section)
    {
        sectionData[section].length = header->sectionHeader[section].SizeOfRawData;
        sectionData[section].data   = new char[sectionData[section].length];

        memcpy(sectionData[section].data,
               (char *) fileMapping+header->sectionHeader[section].PointerToRawData,
               sectionData[section].length);
    }

    // Make sure that there are import entries

    DWORD importVirtualAddress = header->optionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;

    if (   !importVirtualAddress
        || !header->optionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size) {
        UnmapViewOfFile(fileMapping);

        return dependencies;
    }

    // Retrieve the import index

    int importIndex = -1;

    for (int section = 0; section < nbOfSections; ++section)
        if(   (header->sectionHeader[section].VirtualAddress <= importVirtualAddress)
           && (importVirtualAddress < header->sectionHeader[section].VirtualAddress+header->sectionHeader[section].SizeOfRawData))
            importIndex = section;

    // Make sure that the import index is valid

    if (importIndex < 0) {
        UnmapViewOfFile(fileMapping);

        return dependencies;
    }

    // Retrieve the import section data

    char *importSectionData = sectionData[importIndex].data;
    DWORD importSectionHeaderVirtualAddress = header->sectionHeader[importIndex].VirtualAddress;

    IMAGE_IMPORT_DESCRIPTOR *importDescriptor = (IMAGE_IMPORT_DESCRIPTOR *) (importSectionData+importVirtualAddress-importSectionHeaderVirtualAddress);

    for (; !IsBadReadPtr(importDescriptor, sizeof(*importDescriptor)) && importDescriptor->Name; ++importDescriptor)
        dependencies << (char *) (importSectionData+importDescriptor->Name-importSectionHeaderVirtualAddress);

    // Release the memory allocated for the sections' data

    for (int section = 0; section < nbOfSections; ++section)
        delete[] sectionData[section].data;

    delete[] sectionData;

    // Get rid of the file mapping for the plugin and return the plugin's
    // dependencies

    UnmapViewOfFile(fileMapping);

    return dependencies;
}
#endif

PluginInfo Plugin::info(  const QString &pPluginFileName
#ifdef Q_WS_WIN
                        , const QStringList &pLoadedPlugins
#endif
                       )
{
#ifdef Q_WS_WIN
//if (name(pPluginFileName) == "CellMLModelRepository")
    QMessageBox::information(0, name(pPluginFileName),  "Loaded plugins:\n - "+pLoadedPlugins.join("\n - ")
                                                       +"\n\nDependencies:\n - "+dependencies(pPluginFileName).join("\n - "));
#endif

//QLibrary lib(fileName(QFileInfo(pPluginFileName).canonicalPath(), "Core"));

//if (name(pPluginFileName) == "CellMLModelRepository")
//    if (!lib.load())
//        QMessageBox::information(0, "Core plugin",  lib.errorString());

    // Return the information associated to the plugin

    PluginInfo pluginInfo;

    typedef PluginInfo (*PluginInfoFunc)();

    PluginInfoFunc pluginInfoFunc = (PluginInfoFunc) QLibrary::resolve(pPluginFileName,
                                                                       QString(name(pPluginFileName)+"PluginInfo").toLatin1().constData());

    if (pluginInfoFunc)
        // The plugin information function was found, so we can extract the
        // information we are after

        pluginInfo = pluginInfoFunc();
    else
        // The plugin information funciton couldn't be found which means that
        // we are not dealing with an OpenCOR plugin

        pluginInfo.type = PluginInfo::Undefined;

//if (name(pPluginFileName) == "CellMLModelRepository")
//    lib.unload();

    return pluginInfo;
}

}
