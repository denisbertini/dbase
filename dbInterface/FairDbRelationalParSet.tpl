#include "FairDbRelationalParSet.h"

#include "TSQLStatement.h"
#include "FairDbConnectionPool.h"
#include "FairDbString.h"
#include "FairDbCache.h"
#include "FairDbTableInterface.h"

ClassImpT(FairDbRelationalParSet,T)

template<typename T>
FairDbRelationalParSet<T>::FairDbRelationalParSet()
  : FairDbGenericParSet<T>(),
  fId(-1)
{
}

template<typename T>
FairDbRelationalParSet<T>::FairDbRelationalParSet(FairDbDetector::Detector_t detid,
              DataType::DataType_t dataid,
              const char* name,
              const char* title,
              const char* context,
              Bool_t ownership)
  : FairDbGenericParSet<T>(detid, dataid, name, title, context, ownership),
  fId(-1)
{
}

template<typename T>
FairDbRelationalParSet<T>::FairDbRelationalParSet(const FairDbRelationalParSet& from)
  : FairDbGenericParSet<T>(from)
{
  SetId(from.fId);
}

template<typename T>
FairDbRelationalParSet<T>& FairDbRelationalParSet<T>::operator=(const FairDbRelationalParSet& from)
{
  if (this == &from) { return *this; }

  FairDbGenericParSet<T>::operator=(from);
  SetId(from.fId);

  return *this;
}

template<typename T>
FairDbRelationalParSet<T>::~FairDbRelationalParSet()
{
}

template<typename T>
void FairDbRelationalParSet<T>::store(UInt_t rid)
{
  if (fId == -1)
  {
    SetId(AllocateNextId());
    if (fId == -1) {
      DBLOG("FairDb", FairDbLog::kInfo)
        << "FairDbRelationalParSet<T>::store()"
        << " refuse to store entitty with Id = -1"
        << std::endl;
      return;
    }
  }

  FairDbGenericParSet<T>::store(rid);
}

template<typename T>
void FairDbRelationalParSet<T>::PurgeCache()
{
  FairDbCache *cache = (FairDbCache*)
    (const_cast<FairDbTableInterface&> (FairDbReader<T>::GetTableInterface()).GetCache());
  if (cache) {
    cache->Purge();
  }
}

template<typename T>
Int_t FairDbRelationalParSet<T>::AllocateNextId()
{
  // needs to be uppercase
  std::string tableName = this->GetTableName();
  bool tableExists = this->fMultConn->TableExists(tableName, this->fDbEntry);
  if (!tableExists) {
    return 0;
  }

  std::unique_ptr<FairDbStatement> statement(this->fMultConn->CreateStatement(this->fDbEntry));
  if (!statement) {
    return -1;
  }

  FairDbConnectionPool::BLock Block(this->fMultConn->CreateStatement(this->fDbEntry), tableName, tableName);
  if ( ! Block.IsBLocked() ) {
    DBLOG("FairDb",FairDbLog::kInfo)<< "FairDbRelationalParSet<T>::AllocateNextId Unable to lock " << tableName << std::endl;
    return -1;
  }

  FairDbString sql;
  sql << "SELECT ID FROM " << tableName << " ORDER BY ID DESC LIMIT 1";
  std::unique_ptr<TSQLStatement> stmt(statement->ExecuteQuery(sql.c_str()));
  statement->PrintExceptions(FairDbLog::kInfo);

  Int_t id = 0;
  if (stmt && stmt->NextResultRow()) {
    id = stmt->GetInt(0) + 1;
  }

  return id;
}

template<typename T>
std::unique_ptr<T> FairDbRelationalParSet<T>::GetById(Int_t id, UInt_t rid)
{
  return T::GetByIndex(id, rid);
}

template<typename T>
std::vector<T> FairDbRelationalParSet<T>::GetByIds(std::vector<Int_t> ids, UInt_t rid)
{
  return T::GetBy(
    [&ids](const T& inst) -> bool
      {
        for (Int_t id : ids)
        {
          if (id == inst.GetId())
          {
            return true;
          }
        }
        return false;
      });
}

template<typename T>
std::unique_ptr<T> FairDbRelationalParSet<T>::FromJsonString(std::string jsonString)
{
  try
  {
    jsoncons::strict_parse_error_handler errorHandler;
    jsoncons::json json = jsoncons::json::parse(jsonString, errorHandler);
    return FromJson(json);
  }
  catch (const jsoncons::parse_error& e)
  {
    std::cout << "FairDbRelationalParSet<T>::FromJsonString" << std::endl;
    std::cout << e.what() << std::endl;
    return nullptr;
  }
}

template<typename T>
std::unique_ptr<T> FairDbRelationalParSet<T>::FromJson(jsoncons::json json)
{
  if (!json.is_object())
  {
    return nullptr;
  }

  T instance;
  instance.FillFromJson(json);
  return std::unique_ptr<T>(new T(instance));
}

template<typename T>
std::vector<T> FairDbRelationalParSet<T>::FromJsonArray(jsoncons::json jsonArray)
{
  if (!jsonArray.is_array())
  {
    return {};
  }

  std::vector<T> result;
  result.reserve(jsonArray.size());

  for (const jsoncons::json& element : jsonArray.array_range())
  {
    std::unique_ptr<T> deserialized(std::move(FromJson(element)));
    if (deserialized)
    {
      result.emplace_back(*deserialized);
    }
  }

  return result;
}

template<typename T>
std::string FairDbRelationalParSet<T>::ToJsonString()
{
  std::ostringstream s;
  s << jsoncons::pretty_print(ToJson());
  return s.str();
}

template<typename T>
jsoncons::json FairDbRelationalParSet<T>::ToJson()
{
  jsoncons::json result;

  StoreToJson(result);

  return result;
}

template<typename T>
jsoncons::json FairDbRelationalParSet<T>::ToJsonArray(std::vector<T> array)
{
  jsoncons::json jsonArray = jsoncons::json::array();

  Int_t count = array.size();
  jsonArray.resize(count);
  for (Int_t i = 0; i < count; i++)
  {
    jsonArray[i] = array[i].ToJson();
  }

  return jsonArray;
}
