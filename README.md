# GameUserSettingsEnhanced
Unreal Engine plugin that provides blueprint accessible GameUserSettings with blueprint variable saving and loading functionality.

The plugin is pretty straight forward. Once added to your projects Plugins folder it should compile and be auto-enabled.

Next in the Content Browser right click then click Blueprint Class and create a new blueprint asset of "GameUserSettingsEnhancedBlueprint" type.

Now within that blueprint add as many blueprint variables as needed that will act as new configuration values that will be properly stored to and loaded from the GameUserSettings.ini file.

Within Project Settings > Engine > General Settings be sure to set "Game User Settings Class" to your new bluperint class.

Now when you call GetGameUserSettings simply cast to your blueprint and get/set your variables freely. Feel free to add getter/setter functions if you want or even add an interface and avoid casting if you like.

All blueprint variable types are supported. This includes arrays, maps, etc.. they will serialize to string safely. Note Objects will only serialize class pointers.

This entirely handles the process of loading, applying, saving, validating, etc.. your blueprint variables the same way C++ would.

In addition to this you can pass blueprint variables off as console variables (CVar) using the Console Variables variable. Once your blueprint is compiled you can edit this map variable to map a blueprint variable into a CVar value. So for example a blueprint variable named "BloomQuality" can me mapped to "r.BloomQuality". It will automatically handle executing the CVar set behavior for you on load.

This is only tested on Unreal Engine 5.7. If you need it for older versions you'll need to handle editing, compiling, and testing that for yourself.
