# Gamedev Helper plugin
Collection of reusable functions and tools used in game development for unreal engine.

## Features:
todo

## Modules:
- Utility - Contains only Blueprint function libraries for operation on string, assets, arrays etc.
- Tester - Plugin internal module for automation tools testing.
- VertexAnimTool - Vertex animation related tools
- ProjectOrganizer - For managing project files and folders, their naming etc.

## API
- UGamedevHelperStringLibrary
    - Normalize
    - Tokenize
    - GetPrefix
    - HasPrefix
    - RemovePrefix
    - GetFirstLetter
    - GetLastLetter
    - GetFirstLetterRemoved
    - GetLastLetterRemoved
    - GetLetterRemoved
    - IsLetters
    - IsDigits (similar to FString::IsNumeric)
    - ContainsLetters
    - ContainsDigits
    - Contains(Dictionary)
    - ContainsOnly(Dictionary)
    - ConvertCase(NamingCase)
- UGamedevHelperAssetLibrary
    - FixupRedirectors
    - SaveAll
    - Save(Selected)
    - Save(Single)
    - Create(asset_type, asset_factory)
    - IsBlueprint
    - GetBlueprintType
    - DisableCollision(selected_assets)
    - Numerize(selected_assets)

- UGamedevTesterTestRunner
    - AddTestCase
    - RegisterFuzzer (optional)
    - GetResult
- UGamedevTesterFuzzer
    - GetRandomString(len, dictionary)
    - GetRandomString(len, format)
    - GetRandomString(format)
    - GetStringFormatFromToken() - private

- UGamedevVertexAnimTool
    - ConfigureStaticMesh(selected_meshes)
    - CongigureStaticMesh(single_mesh)
    - ConfigureTexture(texture_type, texture_assets) - texture_type can be UV or Normal 

- UGamedevProjectOrganizer
    - FixAssetNames(convention_settings, selected_assets)
    - FixAssetName(convention_settings, single_asset)
    - GetAssetNamingSettings(fassetdata)
    - todo: specific asset settings (texture, material, static_mesh etc)