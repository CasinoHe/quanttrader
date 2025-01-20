-- config sample for strategy service

tws_service = {
	host = "127.0.0.1",
	port = 8001,
	clientid = 0,

	retry_interval = 5000,  -- retry interval in milliseconds
	wait_timeout = 10,      -- wait data from TWS timeout in milliseconds
	update_config_interval = 60000, -- update config interval in milliseconds
	record_log = 0,         -- record log or not
	stop_flag = 0,          -- stop flag
}

stock_trade_service = {
	broker_service = "tws",
	broker_config = "this",
	back_test_config = "this",
}

back_test_service = {
	wait_new_strategy_interval = 1000,
}