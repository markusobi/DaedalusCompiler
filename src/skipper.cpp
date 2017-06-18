#include "skipper_def.hpp"
#include "config.hpp"

namespace parser
{
    BOOST_SPIRIT_INSTANTIATE(skipper_type, iterator_type, x3::unused_type)
}
