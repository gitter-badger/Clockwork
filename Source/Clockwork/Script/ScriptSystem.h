
#include "../Core/Object.h"

#pragma once

namespace Clockwork
{
  class ScriptSystem : public Object
  {
      OBJECT(ScriptSystem);

  public:

      /// Construct.
      ScriptSystem(Context* context);
      /// Destruct.
      virtual ~ScriptSystem();
  };

}
