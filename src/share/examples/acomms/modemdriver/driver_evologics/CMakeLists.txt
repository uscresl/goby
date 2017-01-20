add_executable(driver_evologics driver_evologics.cpp)
target_link_libraries(driver_evologics goby_acomms ${Boost_LIBRARIES} ${PROTOBUF_LIBRARIES})
install(TARGETS driver_evologics RUNTIME DESTINATION bin)