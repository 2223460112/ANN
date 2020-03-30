/*
 * Layer.hpp
 *
 *  Created on: 20200306
 *      Author: Z2J
 */

#ifndef LIB_LAYER_HPP_
#define LIB_LAYER_HPP_

#include "../lib/Unit.hpp"

class BaseLayer {
protected:
	UnitPool *Pool;
	uint32_t Input, Output;
public:
	uint32_t Units;
	virtual ushort TYPE() {
		return __BASE__;
	}
public:
	BaseLayer() {
	}
	virtual ~BaseLayer() {
	}
	virtual void FIXDlist(uint32_t _Dlist) {
		for (uint32_t i = 0; i < Pool->Dlist[Units].size(); i++)
			Pool->AtDlist(Units, i).FIXDlist(_Dlist);
	}
	virtual void save(FILE *file) {
		fiputc(Input, file)
		fiputc(Output, file)
		fiputc(Units, file)
	}
	virtual void FIXload(uint32_t _Input, uint32_t _Output,uint32_t _Dlist){
		for (uint32_t i = 0; i < Pool->Dlist[Units].size(); i++)
			Pool->AtDlist(Units, i).FIXDlist(_Dlist);
		for (uint32_t i = 0; i < Pool->Dlist[Units].size(); i++)
			Pool->AtDlist(Units, i).FIXload(_Input,_Output);
	}
	BaseLayer(FILE *file, UnitPool *_Pool) {
		figetc(Input, file)
		figetc(Output, file)
		figetc(Units, file)
		Pool = _Pool;
	}
public:
	virtual void calc() {
	}
	virtual void train() {
	}
	virtual void trainT(MatrixXd targetV) {
	}
	virtual void calcdW(double Learnrate) {
	}
	virtual void modify() {
	}
};
class DNNOutputLayer: public BaseLayer {
public:
	ushort TYPE() {
		return __DNN_OUTPUT__;
	}
public:
	DNNOutputLayer() {
	}
	DNNOutputLayer(FILE *file, UnitPool *_Pool) {
		figetc(Input, file)
		figetc(Output, file)
		figetc(Units, file)
		Pool = _Pool;
	}
	DNNOutputLayer(UnitPool *_Pool, uint32_t _Input, uint32_t _Output,
			StepFunc::func _step, LossFunc::func _loss) {
		Pool = _Pool;
		uint32_t LayerID;
		LayerID = Pool->newLayer();
		Input = _Input, Output = _Output;
		Pool->Pool[LayerID].resize(
				Pool->Mats[Output].rows() * Pool->Mats[Output].cols());
		for (uint32_t i = 0, k = 0; i < Pool->Mats[Output].rows(); i++)
			for (uint32_t j = 0; j < Pool->Mats[Output].cols(); j++, k++) {
				Pool->Pool[LayerID][k] = new DNNOutputUnit(_Pool, _Input,
						_Output, std::make_pair(i, j), _step, _loss);
			}
		Units = Pool->UpdateToDlist(LayerID);
	}
	~DNNOutputLayer() {
	}
public:
	void calc() {
		for (uint32_t i = 0; i < Pool->Dlist[Units].size(); i++)
			Pool->AtDlist(Units, i).calc(Input, Output);
	}
	void trainT(MatrixXd targetV) {
		for (uint32_t i = 0; i < Pool->Dlist[Units].size(); i++)
			Pool->AtDlist(Units, i).trainT(
					targetV(Pool->AtDlist(Units, i).OutputPos.first,
							Pool->AtDlist(Units, i).OutputPos.second));
	}
	void calcdW(double Learnrate) {
		for (uint32_t i = 0; i < Pool->Dlist[Units].size(); i++)
			Pool->AtDlist(Units, i).calcdW(Input, Learnrate);
	}
	void modify() {
		for (uint32_t i = 0; i < Pool->Dlist[Units].size(); i++)
			Pool->AtDlist(Units, i).modify();
	}
};
class DNNInnerLayer: public DNNOutputLayer {
public:
	ushort TYPE() {
		return __DNN_INNER__;
	}
public:
	DNNInnerLayer(FILE *file, UnitPool *_Pool) {
		figetc(Input, file)
		figetc(Output, file)
		figetc(Units, file)
		Pool = _Pool;
	}
	DNNInnerLayer(UnitPool *_Pool, uint32_t _Input, uint32_t _Output,
			StepFunc::func _step) {
		Pool = _Pool;
		uint32_t LayerID;
		LayerID = Pool->newLayer();
		Input = _Input, Output = _Output;
		Pool->Pool[LayerID].resize(
				Pool->Mats[Output].rows() * Pool->Mats[Output].cols());
		for (uint32_t i = 0, k = 0; i < Pool->Mats[Output].rows(); i++)
			for (uint32_t j = 0; j < Pool->Mats[Output].cols(); j++, k++) {
				Pool->Pool[LayerID][k] = new DNNInnerUnit(_Pool, _Input,
						_Output, std::make_pair(i, j), _step);
			}
		Units = Pool->UpdateToDlist(LayerID);
	}
	~DNNInnerLayer() {
	}
public:
	void train() {
		for (uint32_t i = 0; i < Pool->Dlist[Units].size(); i++)
			Pool->AtDlist(Units, i).train();
	}
};

#endif /* LIB_LAYER_HPP_ */
