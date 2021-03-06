#include <iostream>
#include <string>
#include <memory>

#include "kangaru.hpp"

/**
 * This example explains advanced use of kangaru and it's components.
 * It covers custom service definition
 */

using namespace std;
using namespace kgr;

struct FlourBag {
	int quantity = 99;
};

struct Bakery;

struct Oven {
	void bake() {
		cout << "Bread baking..." << endl;
	}
};

struct Baker {
	Baker(unique_ptr<FlourBag> myFlour) : flour{move(myFlour)} {}
	void makeBread(Bakery& b, Oven* o);
	
private:
	unique_ptr<FlourBag> flour;
};

struct Bakery {
	Bakery(Oven* myOven) : oven{myOven} {}
	
	void start(shared_ptr<Baker> baker) {
		baker->makeBread(*this, oven);
	}
	
	string name;
	
private:
	Oven* oven;
};

void Baker::makeBread(Bakery& b, Oven* o) {
	flour->quantity -= 8;
	o->bake();
	cout << "Bread baked at the \"" << b.name << "\" bakery. " << flour->quantity << " cups of flour left." << endl;
}

/**
 * Service Definitions
 */

// FlourBag service definition. 
struct FlourBagService {
	// constructor. Receives a fully constructed flour bag to contain.
	FlourBagService(unique_ptr<FlourBag> f) : flour{move(f)} {}
	
	// construct method. Receives dependencies of a FlourBag. In this case none.
	static FlourBagService construct() {
		return unique_ptr<FlourBag>{new FlourBag};
	}
	
	// forward method. This method define how the service is injected.
	unique_ptr<FlourBag> forward() {
		return move(flour);
	}
	
private:
	unique_ptr<FlourBag> flour;
};

// Oven service definition. 
struct OvenService : Single {
	// constructor. Receives a fully constructed oven to contain.
	OvenService(unique_ptr<Oven> o) : oven{move(o)} {}
	
	// construct method. Receives dependencies of a Oven. In this case none.
	static OvenService construct() {
		return unique_ptr<Oven>{new Oven};
	}
	
	// forward method. This method define how the service is injected.
	// It's virtual because other services can override this one.
	// See example 4 for more detail.
	virtual Oven* forward() {
		return oven.get();
	}
	
private:
	unique_ptr<Oven> oven;
};

// Baker service definition. 
struct BakerService : Single {
	// constructor. Receives a fully constructed baker to contain.
	BakerService(shared_ptr<Baker> b) : baker{move(b)} {}
	
	// construct method. Receives dependencies of a Baker.
	static BakerService construct(FlourBagService flourBag) {
		// dependencies are injected with the forward method.
		return make_shared<Baker>(flourBag.forward());
	}
	
	// forward method. This method define how the service is injected.
	// It's virtual because other services can override this one.
	// See example 4 for more detail.
	virtual shared_ptr<Baker> forward() {
		return baker;
	}
	
private:
	shared_ptr<Baker> baker;
};

// Bakery service definition. 
struct BakeryService {
	// constructor. Receives a fully constructed bakery to contain.
	BakeryService(Bakery b) : bakery{move(b)} {}
	
	// construct method. Receives dependencies of a Baker.
	// We have to receive the OvenService has a reference because it's single.
	static BakeryService construct(OvenService& oven) {
		// dependencies are injected with the forward method.
		return Bakery{oven.forward()};
	}
	
	// forward method. This method define how the service is injected.
	Bakery forward() {
		return move(bakery);
	}
	
private:
	Bakery bakery;
};

int main()
{
	Container container;
	
	// The return type of 'service<BakeryService>' is the same as the BakeryService::forward return type.
	Bakery bakery = container.service<BakeryService>();
	bakery.name = "Le bon pain";
	
	// The return type of 'service<BakerService>' is the same as the BakerService::forward return type.
	shared_ptr<Baker> baker = container.service<BakerService>();
	
	bakery.start(baker);
	
	return 0;
}
