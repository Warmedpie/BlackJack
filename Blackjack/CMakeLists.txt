# CMakeList.txt : CMake project for Blackjack, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Blackjack "Blackjack.cpp" "Blackjack.h" "Dealer.h" "Dealer.cpp" "Cards/Card.h" "Cards/Deck.h" "Cards/Deck.cpp"  "Strategy/BasicStrategy.h" "Strategy/BasicStrategy.cpp" "Strategy/Strategy.h" "Strategy/HiLoCounter.h" "Strategy/HiLoCounter.cpp"  "Strategy/BlackjackBot.h" "Strategy/BlackjackBot.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Blackjack PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
