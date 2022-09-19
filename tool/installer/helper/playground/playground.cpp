#include <iostream>
#include "install_helper.h"

int main() {

	wchar_t buffer[256]{};
	int r = External_GetNPPInstalledDirectoryPath(buffer, 256);
}