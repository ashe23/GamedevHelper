# Gamedev Helper plugin
Collection of reusable functions and tools used in game development for unreal engine.

## Features:
todo

## Modules:
- GamedevHelperCore
- GamedevHelperEditor

## API
- UGamedevHelperStringLibrary
    - GetFirstLetter
    - GetLastLetter
    - RemoveFirstLetter
    - RemoveLastLetter
    - Intersection
    - Union
    - SymmetricDifference
    - Difference
    - IsSubSet
    - Contains
    - ContainsOnly
    - ContainsOnlyLetters
    - ContainsOnlyDigits
    - ContainsLetters
    - ContainsDigits

- UGamedevHelperArrayLibrary
    - ShuffWithSeed

- AssetRenamingClass
    - ConvertCase(NamingCase) 
    - Normalize 
    - Tokenize 
    - GetPrefix 
    - HasPrefix 
    - RemovePrefix 
    - Numerize(selected_assets)

- UGamedevHelperAssetLibrary
    - SaveAll
    - FixupRedirectors
    - IsBlueprint
    - GetBlueprintType
    - DisableCollision(selected_assets)
    - VATConfigureStaticMesh(selected_meshes)
    - VATConfigureTexture(texture_type, texture_assets) - texture_type can be UV or Normal 

- UGamedevTesterTestRunner
    - AddTestCase
    - RegisterFuzzer (optional)
    - ExpectTrue
    - ExpectFalse
    - GetResult

- UGamedevTesterFuzzer
    - GetRandomString(len, dictionary)
    - GetRandomString(len, format)
    - GetRandomString(format)
    - GetStringFormatFromToken() - private

- UGamedevProjectOrganizer
    - FixAssetNames(convention_settings, selected_assets)
    - FixAssetName(convention_settings, single_asset)
    - GetAssetNamingSettings(fassetdata)
    - todo: specific asset settings (texture, material, static_mesh etc)