target_sources(${TARGET_NAME} PRIVATE
	led.c
	stm32h7_hal_eth_compat.c
)

add_subdirectory(boardconfig)