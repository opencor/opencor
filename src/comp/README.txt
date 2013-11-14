Files in this folder come in pairs and are used to enable/disable a particular
compiler warning. Originally, the idea was to use them as follows (based on
src/plugins/plugin.h):

    ...
    namespace OpenCOR {
    ...
    #include "disableunusedvariablewarning.inl"
        static const char *SettingsOrganization = "Physiome";
        static const char *SettingsApplication  = "OpenCOR";
        static const char *SettingsPlugins      = "Plugins";
    #include "enableunusedvariablewarning.inl"
    ...
    }   // namespace OpenCOR
    ...

However, gcc needs to reach the closing curly bracket to know whether a
particular variable has been used. This means that the above code will generate
a warning despite our attempt at suppressing it. The solution is therefore to
have something like:

    ...
    #include "disableunusedvariablewarning.inl"
    namespace OpenCOR {
        static const char *SettingsOrganization = "Physiome";
        static const char *SettingsApplication  = "OpenCOR";
        static const char *SettingsPlugins      = "Plugins";
    }   // namespace OpenCOR
    #include "enableunusedvariablewarning.inl"
    ...
    namespace OpenCOR {
    ...
    }   // namespace OpenCOR
    ...

Note that we have two namespace statements. The idea is indeed to isolate the
first one to make sure that the enabling/disabling of a particular warning is
only applied to what we know is 'safe' to ignore.

This issue obviously doesn't arise if we are not within a namespace, a function,
etc. More information on suppressing warnings can be found at:
https://svn.boost.org/trac/boost/wiki/Guidelines/WarningsGuidelines
