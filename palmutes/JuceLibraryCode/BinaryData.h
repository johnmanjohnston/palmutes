/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   guitar_png;
    const int            guitar_pngSize = 171941;

    extern const char*   Palm_Muteshighres_png;
    const int            Palm_Muteshighres_pngSize = 118514;

    extern const char*   SpaceGroteskBold_ttf;
    const int            SpaceGroteskBold_ttfSize = 86520;

    extern const char*   SpaceGroteskLight_ttf;
    const int            SpaceGroteskLight_ttfSize = 86616;

    extern const char*   SpaceGroteskMedium_ttf;
    const int            SpaceGroteskMedium_ttfSize = 86616;

    extern const char*   SpaceGroteskRegular_ttf;
    const int            SpaceGroteskRegular_ttfSize = 86592;

    extern const char*   SpaceGroteskSemiBold_ttf;
    const int            SpaceGroteskSemiBold_ttfSize = 86576;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 7;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
