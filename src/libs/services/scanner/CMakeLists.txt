
add_library(lmsscanner SHARED
	impl/ScannerService.cpp
	impl/ScannerStats.cpp
	impl/ScanStepCheckDuplicatedDbFiles.cpp
	impl/ScanStepCompact.cpp
	impl/ScanStepComputeClusterStats.cpp
	impl/ScanStepDiscoverFiles.cpp
	impl/ScanStepOptimize.cpp
	impl/ScanStepRemoveOrphanDbFiles.cpp
	impl/ScanStepScanFiles.cpp
	)

target_include_directories(lmsscanner INTERFACE
	include
	)

target_include_directories(lmsscanner PRIVATE
	include
	)

target_link_libraries(lmsscanner PRIVATE
	lmsdatabase
	lmsmetadata
	lmsrecommendation
	lmscore
	)

target_link_libraries(lmsscanner PUBLIC
	std::filesystem
	Wt::Wt
	)

install(TARGETS lmsscanner DESTINATION ${CMAKE_INSTALL_LIBDIR})

