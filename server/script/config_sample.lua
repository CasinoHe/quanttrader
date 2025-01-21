-- config sample for strategy service

tws_service = {
	host = "127.0.0.1",
	port = 8001,
	clientid = 0,

	retry_interval = 5000,  -- retry interval in milliseconds
	wait_timeout = 10,      -- wait data from TWS timeout in milliseconds, not updated unless restart quanttrader, TODO
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
	update_config_interval = 60000,
	wait_timeout = 1000,                   -- timeout on each threads in back test loop in milliseconds
	new_test = "apple_daily,msft_weekly",  -- config table name, split by comma
	-- restart_test = "apple_daily,msft_weekly", -- which test to restart
	-- stop_test = "apple_daily,msft_weekly",  -- which test to stop
}

apple_daily = {
	version = 1,

}

msft_weekly = {
	version = 1,

}
