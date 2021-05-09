#define STRINGIZE(a) #a 
#define VERSION_STRING(major, minor, revision, build) STRINGIZE(major.minor.revision.build)
