#include <iostream>
#include <string>
#include <memory>

#include "kangaru.hpp"

/**
 * This example explains the very basic use of kangaru.
 * It covers dependencies, Single services and basic use of the container.
 */

using namespace std;
using namespace kgr;

struct PathProvider {
	string path;
};

struct PathPrinter {
	// PathPrinter needs a PathProvider
	PathPrinter(PathProvider& _pathProvider) : pathProvider{_pathProvider} {}
	
	void print() {
		cout << pathProvider.path << endl;
	}
	
private:
	PathProvider& pathProvider;
};

// This is our service definitions
// PathProviderService is a single service of PathProvider
struct PathProviderService : SingleService<PathProvider> {};

// PathPrinterService is a (not single) service of PathProvider and has a PathProviderService as dependency
struct PathPrinterService : Service<PathPrinter, Dependency<PathProviderService>> {};

int main()
{
	Container container;
	
	// a PathProvider is provided for every printer
	auto printer1 = container.service<PathPrinterService>();
	auto printer2 = container.service<PathPrinterService>();
	auto printer3 = container.service<PathPrinterService>();
	
	auto& provider = container.service<PathProviderService>();
	
	provider.path = "/home/test";
	
	// every printer will print /home/test, because every printer has the same instance of PathProvider
	printer1.print();
	printer2.print();
	printer3.print();
	
	return 0;
}
