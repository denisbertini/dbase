/**
* @file FairDbWtGenericResource.cxx
* @brief FairDb web resource class. Generated automatically
* @author Generator by Evgeny Lavrik <evgeny.lavrik@uni-tuebingen.de>
* @date 30.11.2017
**/

#include "FairDbWtGenericResource.h"
#include "FairDbCache.h"

template<typename T>
FairDbWtGenericResource<T>::FairDbWtGenericResource(Wt::WServer& server, Wt::WObject *parent)
  : FairDbWtBaseResource(server, parent)
{
  std::string prefix = std::string("/") + boost::typeindex::type_id<T>().pretty_name();
  AddEndpoint(prefix + "/GetById", FairDbUserRole::kGuest, boost::bind(&FairDbWtGenericResource::GetById, this, _1, _2, _3, _4));
  AddEndpoint(prefix + "/GetAllVersions", FairDbUserRole::kGuest, boost::bind(&FairDbWtGenericResource::GetAllVersions, this, _1, _2, _3, _4));
  AddEndpoint(prefix + "/GetArray", FairDbUserRole::kGuest, boost::bind(&FairDbWtGenericResource::GetArray, this, _1, _2, _3, _4));
  AddEndpoint(prefix + "/GetAll", FairDbUserRole::kGuest, boost::bind(&FairDbWtGenericResource::GetAll, this, _1, _2, _3, _4));
  AddEndpoint(prefix + "/Store", FairDbUserRole::kOperator, boost::bind(&FairDbWtGenericResource::Store, this, _1, _2, _3, _4));
  AddEndpoint(prefix + "/StoreArray", FairDbUserRole::kOperator, boost::bind(&FairDbWtGenericResource::StoreArray, this, _1, _2, _3, _4));
}

template<typename T>
void FairDbWtGenericResource<T>::GetById(const Wt::Http::Request& request, Json::Value requestData, Wt::Http::Response& response, Json::Value &responseData)
{
  Int_t id = requestData.get("Id", -1).asInt();
  UInt_t rid = requestData.get("Rid", (UInt_t)ValTimeStamp()).asUInt();
  std::unique_ptr<T> instance = T::GetById(id, rid);

  if (instance)
  {
    responseData["data"] = instance->ToJson();
  } else {
    responseData["error"] = "Not found";
  }
}

template<typename T>
void FairDbWtGenericResource<T>::GetAllVersions(const Wt::Http::Request& request, Json::Value requestData, Wt::Http::Response& response, Json::Value &responseData)
{
  Int_t id = requestData.get("Id", -1).asInt();
  std::unique_ptr<T> instance = T::GetById(id, ValTimeStamp());
  if (instance)
  {
    std::vector<T> array = instance->GetAllVersions();
    Json::Value jsonArray = T::ToJsonArray(array);
    responseData["data"] = jsonArray;
  } else {
    responseData["error"] = "Not found";
  }
}

template<typename T>
void FairDbWtGenericResource<T>::GetArray(const Wt::Http::Request& request, Json::Value requestData, Wt::Http::Response& response, Json::Value &responseData)
{
  UInt_t rid = requestData.get("Rid", (UInt_t)ValTimeStamp()).asUInt();
  Json::Value jsonIdsArray = requestData.get("Ids", Json::Value(Json::arrayValue));
  Int_t idsCount = jsonIdsArray.size();

  if (!idsCount)
  {
    responseData["data"] = Json::Value(Json::arrayValue);
    return;
  }

  std::vector<Int_t> ids(idsCount);
  for (Int_t i = 0; i < idsCount; i++)
  {
    ids[i] = jsonIdsArray.get(i, -1).asInt();
  }

  std::vector<T> array = T::GetByIds(ids, rid);
  Json::Value jsonArray = T::ToJsonArray(array);
  responseData["data"] = jsonArray;
}

template<typename T>
void FairDbWtGenericResource<T>::GetAll(const Wt::Http::Request& request, Json::Value requestData, Wt::Http::Response& response, Json::Value &responseData)
{
  UInt_t rid = requestData.get("Rid", (UInt_t)ValTimeStamp()).asUInt();
  std::vector<T> array = T::GetAll(rid);
  Json::Value jsonArray = T::ToJsonArray(array);
  responseData["data"] = jsonArray;
}

template<typename T>
void FairDbWtGenericResource<T>::Store(const Wt::Http::Request& request, Json::Value requestData, Wt::Http::Response& response, Json::Value &responseData)
{
  UInt_t rid = requestData.get("Rid", (UInt_t)ValTimeStamp()).asUInt();
  Json::Value object = requestData.get("Object", Json::Value(Json::nullValue));
  std::unique_ptr<T> instance = T::FromJson(object);
  if (instance)
  {
    std::string prefix = boost::typeindex::type_id<T>().pretty_name();
    std::string logTitle = prefix + "::Store() from " + request.clientAddress();
    instance->SetLogTitle(logTitle);
    instance->store(rid);
    T::PurgeCache();
  } else {
    responseData["error"] = "Object: wrong type, must be json object";
  }
}

template<typename T>
void FairDbWtGenericResource<T>::StoreArray(const Wt::Http::Request& request, Json::Value requestData, Wt::Http::Response& response, Json::Value &responseData)
{
  UInt_t rid = requestData.get("Rid", (UInt_t)ValTimeStamp()).asUInt();
  Json::Value jsonArray = requestData.get("Array", Json::Value(Json::nullValue));

  std::vector<T> array = T::FromJsonArray(jsonArray);
  if (array.size())
  {
    std::string prefix = boost::typeindex::type_id<T>().pretty_name();
    std::string logTitle = prefix + "::StoreArray() from " + request.clientAddress();
    T::StoreArray(array, rid, logTitle);
    T::PurgeCache();
  } else {
    responseData["error"] = "Array: null or not an array";
  }
}
