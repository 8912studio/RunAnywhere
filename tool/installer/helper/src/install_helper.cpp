#include "install_helper.h"
#include "vs_detecting.h"

int __cdecl IsVisualStudioInstalled() {
	return DetectIfVisualStudioInstalled();
}