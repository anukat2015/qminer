
#include "qminer_pbs.h"

namespace TQm {

	using namespace TQm::TStorage;
	using namespace glib;

	/// Add new record
	uint64 TStorePbBlob::AddRec(const PJsonVal& RecVal) {
		return -1;
	}

	/// Update existing record
	void TStorePbBlob::UpdateRec(const uint64& RecId, const PJsonVal& RecVal) {
	}


	/// Return iterator over store
	PStoreIter TStorePbBlob::GetIter() const {
		if (Empty()) { return TStoreIterVec::New(); }
		return DataMemP ?
			TStoreIterVec::New(DataMem.GetFirstValId(), DataMem.GetLastValId(), true) :
			TStoreIterHash<THash<TUInt64, TPgBlobPt>>::New(RecIdBlobPtH);
	}

	/// Helper function for returning JSON definition of store
	PJsonVal TStorePbBlob::GetStoreJson(const TWPt<TBase>& Base) const {
		PJsonVal Result = TStore::GetStoreJson(Base);
		Result->AddToObj("name", this->GetStoreNm());
		if (WndDesc.WindowType != TStoreWndType::swtNone) {
			PJsonVal WindowJson = TJsonVal::NewObj();
			WindowJson->AddToObj("type", WndDesc.WindowType == TStoreWndType::swtLength ? "length" : "time");
			WindowJson->AddToObj("size", (int)WndDesc.WindowSize);
			if (WndDesc.WindowType == TStoreWndType::swtTime) {
				WindowJson->AddToObj("timeField", WndDesc.TimeFieldNm);
			}
			Result->AddToObj("window", WindowJson);
		}
		return Result;
	}

	/// Save part of the data, given time-window
	int TStorePbBlob::PartialFlush(int WndInMsec) {
		DataBlob->PartialFlush(WndInMsec);
		return 0;
	}
	
	/// Retrieve performance statistics for this store
	PJsonVal TStorePbBlob::GetStats() {
		PJsonVal res = TJsonVal::NewObj();
		res->AddToObj("name", GetStoreNm());
		res->AddToObj("storage", DataBlob->GetStats());
		return res;
	}
}