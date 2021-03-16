//Tom Magdaci 

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
	DefaultIO* dio;
	Command** cmdsArr;
	size_t cmdsArrSize;
    HybridAnomalyDetector* hAD;
    ClientData* c;
public:
	CLI(DefaultIO* dio);
	void start();
	virtual ~CLI();
private:
    void helperCreateAndInsertCmdsToArr();
    string menuCreate();
};

#endif /* CLI_H_ */
