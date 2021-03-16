//Tom Magdaci 

#include "CLI.h"

CLI::CLI(DefaultIO* dio): dio(dio){
    this->cmdsArrSize = 5;
    this->cmdsArr = new Command*[this->cmdsArrSize]();
        //Point** pArr = new Point*[(*this).v->size()]();
    helperCreateAndInsertCmdsToArr();
}

void CLI::helperCreateAndInsertCmdsToArr(){
    this->hAD = new HybridAnomalyDetector();
    this->c = new ClientData();
    *(this->cmdsArr) = new UploadCommand(this->dio, this->hAD , this->c);
    *(this->cmdsArr+1) = new CorrelationReplaceCommand(this->dio, this->hAD );
    *(this->cmdsArr+2) = new AnomalyDetectionCommand(this->dio, this->hAD , this->c);
    *(this->cmdsArr+3) = new AnomaliesResultsCommand(this->dio, this->hAD , this->c);
    *(this->cmdsArr+4) = new AnalyzeResultsCommand(this->dio, this->hAD , this->c);
}

string CLI::menuCreate() {
    string text = "Welcome to the Anomaly Detection Server.\nPlease choose an option:\n";
    int i=0;
    for (;i<this->cmdsArrSize;++i){
        text = text + to_string(i+1) + "." + (*(this->cmdsArr+i))->getDescription() + "\n";
    }
    text = text + "6.exit\n";
    return text;
}

enum commandsIndexes {upload=1,settings=2,detection=3,results=4,analyzeResults=5,exitcmd=6};

void CLI::start(){
    this->dio->write(menuCreate());
    float fArr[1];
    this->dio->read(fArr);
    while(fArr[0]<exitcmd && fArr[0] >= upload) {
        (*(this->cmdsArr + ((int)(fArr[0]-1))))->execute();
        this->dio->write(menuCreate());
        this->dio->read(fArr);
    }
}


CLI::~CLI() {
    if (this->hAD!= nullptr){
        delete this->hAD;
    }
    if (this->c!= nullptr){
        delete this->c->aR;
    }
    releaseArr(this->cmdsArr,this->cmdsArrSize);
}

