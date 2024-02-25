#pragma once

#include <SFML/System.hpp>
#include <cassert>

namespace mmt_gd
{
/**
 * \brief print error message with file and line number
 */
#define ffErrorMsg(Text)                                                              \
    {                                                                                 \
        sf::err() << __FILE__ << " Line: " << __LINE__ << " " << (Text) << std::endl; \
    }


#ifdef _DEBUG
/**
 * \brief print error message and assert
 */
#define ffAssertMsg(Expression, Text)                                                     \
    {                                                                                     \
        if (!(Expression))                                                                \
        {                                                                                 \
            sf::err() << __FILE__ << " Line: " << __LINE__ << " " << (Text) << std::endl; \
            assert(Expression);                                                           \
        }                                                                                 \
    }
#else
#define ffAssertMsg(Expression, Text)
#endif
} // namespace mmt_gd
