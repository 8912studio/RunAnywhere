#define STRINGIZE(a) #a 
#define VERSION_STRING(major, minor, revision, build) STRINGIZE(major.minor.revision.build)
#define YEAR_STRING(year) STRINGIZE(year)
