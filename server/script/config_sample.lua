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
	-- stop_runner = "apple_daily,msft_weekly",  -- which test to stop
	new_runner = "apple_daily,msft_weekly",  -- config table name, split by comma
	-- restart_runner = "apple_daily,msft_weekly", -- which test to restart
	stop_flag = 0,
}

apple_daily = {
	version = 1,                            -- before each operation, increment the version number
	runner_name = "StockRunner",
	strategy_name = "slope_strategy",
	data_series = "daily",
	daily_ticker = "AAPL",
	daily_type = "historical",
	daily_start_date = "2024-08-01 00:00:00",
	daily_end_date = "now",
	daily_security_type = "STK",
	daily_exchange = "SMART",
	daily_currency = "USD",
	daily_use_rth = false,
	daily_timezone = "America/New_York",
	daily_what = "TRADES",
	-- Legal bar type are: 1 secs, 5 secs, 10 secs, 15 secs, 30 secs, 1 min, 2 mins, 3 mins, 5 mins, 10 mins, 15 mins
	-- 20 mins, 30 mins, 1 hour, 2 hours, 3 hours, 4 hours, 8 hours, 1 day, 1W, 1M
	daily_bar_type = "1 day",
}


msft_weekly = {
	version = 1,                            -- before each operation, increment the version number
	runner_name = "StockRunner",
	strategy_name = "slope_strategy",
	symbol = "MSFT",
	start_date = "2016-01-01 00:00:00",
	end_date = "2024-12-31 23:59:59",
	timezone = "America/New_York",
}
