/*
 * PGcommand.h
 *
 *  Builder pattern used here.
 *      Author: pasquale
 */

#ifndef PGCOMMAND_H_
#define PGCOMMAND_H_
#include <iostream>
#include <string.h>
class PGcommand {


public:
	PGcommand():paramNum(0),command(NULL),oids(NULL),paramValues(NULL){};

	//PGcommand(const unsigned int paramNum, const char *command, const unsigned int *oids, const char **paramValues)
	//:paramNum(paramNum),command(command),oids(oids),paramValues(paramValues){};
	virtual ~PGcommand()
	{delete[] oids; for(unsigned int i=0;i<paramNum;i++){delete[] paramValues[i];} delete [] paramValues;};

	unsigned int paramNum;
	const char *command;
	unsigned int *oids;
	char **paramValues;

	void printToStdOut(){
		using namespace std;
		cout<<"-----PGCommand---------"<<endl;
		cout<<paramNum<<endl;
		cout<<command<<endl;
		//cout<<paramValues[0]<<endl;

		for(int i=0;i<4;i++)cout<<"paramValue: "<<paramValues[i]<<endl;
		for(int i=0;i<4;i++)cout<<"oids: "<<oids[i]<<endl;

		cout<<"------------------"<<endl;

	}


};

#include <vector>
#include <stdexcept>

class PGcommandBuilder{

private:

	PGcommand *product;

	int paramNum;
	const char *command;

	std::vector<char*> parValues;
	std::vector<unsigned int> oids;
public:
	PGcommandBuilder():paramNum(0),command(NULL){product=new PGcommand();};
	virtual ~PGcommandBuilder(){delete product;};


	PGcommandBuilder* reset(){
		product=new PGcommand();
		parValues.clear();
		oids.clear();
		return this;
	}

	PGcommandBuilder* setParamNum(int  parNum){
		if(parNum<1){
			throw std::invalid_argument("can't set a value <1");
		}
		this->paramNum=parNum;

		return this;
	}
	PGcommandBuilder* setPGCommand(const char* pgCom){
		command=pgCom;
		return this;
	}
	PGcommandBuilder* addOid(unsigned int oid){
		oids.push_back(oid);
		return this;
	}

	PGcommandBuilder* addParVal(char* val){
		parValues.push_back(val);
		return this;
	}

	PGcommand* build(){
		if(paramNum!=parValues.size() || paramNum!=oids.size()){
			std::cerr<<"parN: "<<paramNum<<"  parVSize: "<<parValues.size()<<"  oidsSiz: "<<oids.size()<<std::endl;
			throw std::logic_error("Illegal state! paramNum!=parValues.size() || paramNum!=oids.size() !!");
		}else{
			//const char **values=&parValues[0];
			//std::cout<<"PGcommand build: values[0]: "<<values[1]<<std::endl;
			//const unsigned int *oidss=&oids[0];
			product->paramNum=this->paramNum;
			//std::cout<<"prod.parNum: "<<product->paramNum<<" this.parn: "<<this->paramNum<<std::endl;
			product->command=this->command;

			product->oids= new unsigned int[paramNum];
			std::copy(&this->oids[0], &this->oids[paramNum], product->oids);


			product->paramValues=new char*[paramNum];
			for (unsigned int i=0;i<paramNum;i++){
				//copy in product.oids from this.oids



				//copy in paramValues from this.parValues
				int len=strlen(parValues[i])+1;
				product->paramValues[i]=new char[len];
				//std::cout<<"sizeofParv: "<<strlen(parValues[i])<<std::endl;
				std::copy(parValues[i], &parValues[i][len], product->paramValues[i]);
			}

			PGcommand *tor=product;
			reset();

			return tor;
		}
	}

};

#endif /* PGCOMMAND_H_ */
