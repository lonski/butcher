#include "actors_database.h"

using namespace cocos2d;

namespace butcher {

ActorDatabase::ActorDatabase()
{
}

bool ActorDatabase::load(const std::string &fn)
{
  Data file_data = FileUtils::getInstance()->getDataFromFile(fn);

  if ( file_data.getSize() == 0 )
  {
    log("ActorDatabase: failed to load file.");
    return false;
  }

  const ActorsData* data = GetActorsData( file_data.getBytes() );

  if ( data == nullptr )
  {
    log("ActorDatabase: failed to load actors.");
    return false;
  }

  auto actors = data->actors();
  for (unsigned i = 0; i < actors->Length(); ++i)
  {
    const ActorData* d = actors->Get(i);
    _actorTemplates[d->id()] = Actor::create(d);
  }

  log("ActorDatabase: loaded %lu actor templates.", _actorTemplates.size());

  return true;
}

}
