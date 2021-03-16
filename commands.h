//Tom Magdaci 

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"
#include <sys/socket.h>
#include <sstream>
#include <ostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include<stdio.h>



using namespace std;

struct ClientData{
    std::vector<AnomalyReport>* aR;
    int n;
};

struct Range {
    long start;
    long end;
};


class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

};

static int writeTo(string text, int fd) {
    char* dataToSend = new char[text.size()+ 1];
    const char* dC = dataToSend;
    strcpy(dataToSend, text.c_str());
    size_t dataSize = text.size();
    int byteswritten = write(fd, dC, dataSize);
}



class SocketIO:public DefaultIO{
public:
    int fd;
    SocketIO(int x):fd(x){}
    virtual string read()throw(const char*){
        int byteread;

        string text;
        char c;

        byteread = ::read(fd,&c,sizeof(char)); //te flag==0 should be verified;
        if (byteread<=0){
            if (byteread<0) {
                throw "reading failure";
            }
            return nullptr; //should be verifie what the return value should be in this case/
        }
        while(c!='\n'){
            text+=c;
            byteread = ::read(fd,&c,sizeof(char));
            if (byteread<=0){
                if (byteread<0) {
                    throw "reading failure";
                }
                return nullptr; //should be verifie what the return value should be in this case/
            }
        }

        return text;
    }
    virtual void write(string text)throw(const char*){
        int byteswritten = ::write(fd,text.c_str(),text.size());
        if (byteswritten<0){
            throw "write failed";
        }
    }

    virtual void write(float f){
        string s;
        std::stringstream ss;
        ss<<f;
        ss>>s;
        int byteswritten = ::write(fd,s.c_str(),s.size());
        if (byteswritten<0){
            throw "write failed";
        }
    }

    virtual void read(float* f){
        string s = read();
        stringstream g(s);
        int x = 0;
        g >> x;
        f[0] = (float) x;
    }
};





// you may edit this class
class Command{
protected:
	DefaultIO* dio;
    string description;
    HybridAnomalyDetector* hAD;
    ClientData* c;
public:
	Command(DefaultIO* dio, string s, HybridAnomalyDetector* newHAD, ClientData* cNew):dio(dio), description(s), hAD(newHAD), c(cNew){}
	virtual void execute()=0;
	virtual ~Command(){}
    virtual string getDescription() {
        return this->description;
    }
protected:
    string readStringFromClient() {
	    return this->dio->read();
	}
	void readFloatArrFromClient(float* f){
	    this->dio->read(f);
	    return;
	}
	void writeStringToClient(string text) {
	    this->dio->write(text);
	}
	void writeFloatToClient(float f) {
	    this->dio->write(f);
	}
};

// implement here your command classes
class UploadCommand:public Command {
public:
    UploadCommand(DefaultIO* dio, HybridAnomalyDetector* newHAD, ClientData* cNew): Command(dio, "upload a time series csv file", newHAD, cNew){
    }
    void clientUploadFileHelper(const char* c, const char* c2){
        writeStringToClient("Please upload your local "+string(c2)+" file.\n");
        string text = readStringFromClient(); //The client will send the file line after line
        ofstream out(c);
        int i = 0;
        while(strcmp(text.c_str(),"done")!=0) {
            ++i;
            out<<text<<endl;
            text = readStringFromClient();
        }
        this->c->n = i;
        out.close();
        writeStringToClient("Upload complete.\n");
    }
    void execute(){
        clientUploadFileHelper("anomalyTrain.csv", "train CSV");
        clientUploadFileHelper("anomalyTest.csv", "test CSV");
    }
};

class CorrelationReplaceCommand:public Command {
public:
    CorrelationReplaceCommand(DefaultIO* dio, HybridAnomalyDetector* newHAD): Command(dio, "algorithm settings", newHAD, nullptr){
    }
    void execute(){
        float fArr[1];
        fArr[0] = -1;
        writeStringToClient("The current correlation threshold is ");
        writeFloatToClient(this->hAD->getThreshold());
        writeStringToClient("\n");
        writeStringToClient("Type a new threshold\n");
        readFloatArrFromClient(fArr);
        while(fArr[0]<0 || fArr[0]>1){
            writeStringToClient("please choose a value between 0 and 1.\n");
            readFloatArrFromClient(fArr);
        };
        this->hAD->setCorrelationThreshold(fArr[0]);
    }
};

class AnomalyDetectionCommand:public Command {
public:
    AnomalyDetectionCommand(DefaultIO* dio, HybridAnomalyDetector* newHAD, ClientData* cNew): Command(dio, "detect anomalies", newHAD, cNew){
    }
    void execute(){
        TimeSeries tsTrain("anomalyTrain.csv");
        TimeSeries tsTest("anomalyTest.csv");
        this->hAD->learnNormal(tsTrain);
        c->aR = new vector<AnomalyReport>();
        //vector<AnomalyReport> temp =
        //(*c->aR).insert((*c->aR).end(), temp.begin(), temp.end());
        (*c->aR) = this->hAD->detect(tsTest);
        writeStringToClient("anomaly detection complete.\n");
    }
};

class AnomaliesResultsCommand:public Command {
public:
    AnomaliesResultsCommand(DefaultIO* dio, HybridAnomalyDetector* newHAD, ClientData* cNew): Command(dio, "display results", newHAD, cNew){
    }
    void execute(){
        vector<AnomalyReport>::iterator it;
        for (it = (*c->aR).begin(); it != (*c->aR).end(); ++it){
            writeStringToClient(to_string((*it).timeStep) + "\t" + " " + (*it).description +"\n");
        }
        writeStringToClient("Done.\n");
    }
};

class AnalyzeResultsCommand:public Command {
    //vector<string> anomaliesRanges;
public:
    AnalyzeResultsCommand(DefaultIO* dio, HybridAnomalyDetector* newHAD, ClientData* cNew): Command(dio, "upload anomalies and analyze results", newHAD, cNew){
    }
    void execute(){
        writeStringToClient("Please upload your local anomalies file.\n");
        vector<string> textClient;
        getAnomaliesFile(textClient);
        writeStringToClient("Upload complete.\n");

        vector<Range> vecAnomaliesRanges;
        turnTextIntoVecAnom(textClient, vecAnomaliesRanges);
        vector<Range> vecReportsRanges;
        anoReportsToVECofRanges(vecReportsRanges);

        //In this stage we have two Range vectors, one with the anomalies and the other one is with our reports.
        int P = vecAnomaliesRanges.size();
        int N = calculateN(vecAnomaliesRanges);

        //a problem: it can happen that now vRR has the next ranges: [1,3] , [6,10] , [1,4]
        //because at the time it was built, the ranges that has been pushed to it has the same descpription
        //sortVecOfRanges(vecReportsRanges);
        //sortVecOfRanges(vecAnomaliesRanges);

        int TP = createVecOfOLorNoOL(vecReportsRanges, vecAnomaliesRanges,1).size();
        //vecRWOL = vecReportsWithoutOverlap
        vector<Range> vecRWOL = createVecOfOLorNoOL(vecAnomaliesRanges, vecReportsRanges,0);
        sortVecOfRanges(vecRWOL);
        vector<Range> vecRWOLMerged = mergeRanges(vecRWOL);
        int FP = vecRWOLMerged.size();
        float TPR = (float) TP/P;
        float FAR = (float) FP/N;
        writeStringToClient("True Positive Rate: ");
        writeFloatToClient(floor(TPR*1000)/1000);
        writeStringToClient("\n");
        writeStringToClient("False Positive Rate: ");
        writeFloatToClient(floor(FAR*1000)/1000);
        writeStringToClient("\n");


    }
private:
    struct RangeComparator {
    public:
        int operator()(const Range& r1, const Range& r2){
            return r1.start < r2.start;
        }
    };
    int sumRanges(vector<Range>& v){
        //someRanges of given vector<Range> (without distinction of overlap)
        int sum;
        vector<Range>::iterator it,end;
        end = v.end();
        for (it=v.begin(); it!=end; ++it){
            sum+=((*it).end-(*it).start)+1;
        }
        return sum;
    }
    /*vector<Range> vecComplementary(vector<Range>& universe, vector<Range>& v){
        vector<Range> vTag;
        vector<Range>::iterator it,it2, end , end2;
        end = universe.end();
        end2 = v.end();
        for (it = universe.begin(); it!=v.end(); ++it) {

        }
        }*/
    vector<Range> createVecOfOLorNoOL(vector<Range>& v1, vector<Range>& v2, int flag){
        //calculate how many ranges in the second vector has overlap with at least one range in vec one;
        //OR create vec from ranges of the second vector that does not have any overlap with ranges from the first vec;
        /*
         * Function guide:
         * flag==1: the function purpose is to compute vec of overlaps.
         *          that means the result vec will contain ranges from v2 that has at least one overlap with range in v1;
         * flag==0: the function purpose is to compute vec of no overlaps.
         *          that means the result vec will contain ranges from v2 that has no overlaps with ranges in v1;
         */
        vector<Range>::iterator it,it2;
        Range temp1,temp2;
        vector<Range> v;
        int b=0;
        for (it = v2.begin(); it!=v2.end(); ++it) {
            b=0;
            temp1 = (*it);
            for (it2=v1.begin(); it2!=v1.end(); ++it2){
                temp2 = (*it2);
                if (temp1.end>=temp2.start && temp1.start<=temp2.start){
                    // temp1.start<=temp2.start<=temp1.end
                    if (flag==1) {
                        v.push_back(temp1);
                    }
                    b=1;
                    break;
                }
                if (temp1.start>=temp2.start && temp1.end<=temp2.end){
                    //temp2.start<=temp1.start && temp1.end<=temp2.end
                    if (flag==1) {
                        v.push_back(temp1);
                    }
                    b=1;
                    break;
                }
                if (temp1.start<=temp2.end && temp1.start>=temp2.start) {
                    // temp2.start<=temp1.start<=temp2.end
                    if (flag==1) {
                        v.push_back(temp1);
                    }
                    b=1;
                    break;
                }
            }
            if((flag==0)&&(b==0)){ //
                v.push_back(temp1);
            }
        }
        return v;
    }
    vector<Range> mergeRanges(vector<Range>& v) {
        //IMPORTANT!!!
        //assuming the vector that has been sent here (as an arg) is sorted by the function bellow "sortVecOfRanges"
        //That means the ranges are sorted by their start time;
        vector<Range> m;
        Range rTemp;
        vector<Range>::iterator it, it2;
        for (it=v.begin(); it!=v.end();){
            //it is procceding according to it2
            rTemp = *it;
            for (it2=next(it);it2!=v.end();++it2){
                //nwo it will be checked whether (it contains it2) or (it cross it2 != emptyset) or (it cross it2 = emptyset)
                if (it2->end <= it->end){
                    //we know that: it2->start >= it->start
                    // it[a,b] it2[c,d] a<=c && d<=b (it contains it2)
                    continue;
                }
                if (it2->start > it->end){
                    // it[a,b] it2[c,d] a<=c && b<c (it cross it2 = emptyset)
                    break;
                }
                //(it cross it2 != emptyset) and b<d
                rTemp.end = (*it2).end;
            }
            it = it2;
            m.push_back(rTemp);
        }
        return m;
    }

    void sortVecOfRanges(vector<Range>& venReportsRanges) {
        sort(venReportsRanges.begin(), venReportsRanges.end(), RangeComparator());
    }
    int calculateN(vector<Range>& vecAnomaliesRanges) {
        int sumRanges=0;
        vector<Range>::iterator it;
        for (it=vecAnomaliesRanges.begin(); it!=vecAnomaliesRanges.end(); ++it){
            sumRanges+= ((*it).end - (*it).start+1);
        }
        return (this->c->n - sumRanges);
    }
    void getAnomaliesFile(vector<string>& textClient){
        //Get the anomalies from the client. each line will be pushed into vec of strings
        textClient.push_back(readStringFromClient()); //The client will send the file line after line
        while (strcmp(textClient[textClient.size()-1].c_str(),"done")!=0) {
            textClient.push_back(readStringFromClient());
        };
        textClient.pop_back(); //pop "done"
    }
    void turnTextIntoVecAnom(vector<string>& text, vector<Range>& vecAnomaliesRanges) {
        //This function purpose is to turn the vec of anomalies as string to vec of Ranges.
        vector<string>::iterator it;
        int i;
        string temp1, temp2;
        Range rTemp;
        for(it=text.begin(); it!=text.end(); ++it){
            i=0;
            for(;i<(*it).size();++i) {
                if ((*it)[i]==','){
                    break;
                }
            }
            temp1 = (*it).substr(0,i);
            temp2 = (*it).substr(i+1,(*it).size()-i-1);
            rTemp.start = stol(temp1,0,10);
            rTemp.end = stol(temp2,0,10);
            vecAnomaliesRanges.push_back(rTemp);
        }
    }
    void anoReportsToVECofRanges(vector<Range>& venReportsRanges) {
        //This function purpose
        vector<AnomalyReport>::iterator it, it2;
        Range rTemp;
        for (it = this->c->aR->begin(); it!=this->c->aR->end();){
            rTemp.start = (*it).timeStep;
            rTemp.end = rTemp.start;
            for (it2 = next(it); it2!=this->c->aR->end(); ++it2){
                if((*it2).description.compare((*it).description)!=0){
                    break;
                }
                if((*it2).timeStep != rTemp.end+1){
                    break;
                }
                ++rTemp.end;
            }
            it = it2;
            venReportsRanges.push_back(rTemp);
        }
    }
};


/*
class ExitCommand:public Command {
public:
    ExitCommand(DefaultIO* dio): Command(dio, "exit"){
    }
    void execute(){

    }
};*/


#endif /* COMMANDS_H_ */
