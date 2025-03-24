-- stock trade service configuration
StockTradeService = {
	broker_service = "tws",
	broker_config = "tws_service.lua",
	cerebro_names = "backtest,live",
	backtest = {
		cerebro_type = "backtest",
		cerebro_config = "backtest_cerebro.lua",
	},
	live = {
		cerebro_type = "live",
		cerebro_config = "live_trade_cerebro.lua",
	},
	data_provider = {
		"tws",          -- only tws works for now
		"localdb",
		"yahoofinance",
	},
	risk_manager = {
		"to_be_implemented",
		"to_be_implemented2",
	},
	engine = "fortune",  -- only forturn for now
	mode = "backtest",  -- backtest or live
}

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

back_test_service = {
	update_config_interval = 60000,
	wait_timeout = 1000,                   -- timeout on each threads in back test loop in milliseconds
	-- stop_runner = "apple_daily,msft_weekly",  -- which test to stop
	new_runner = "apple_slope,msft_slope",  -- config table name, split by comma
	-- restart_runner = "apple_daily,msft_weekly", -- which test to restart
	stop_flag = 0,
}

apple_slope = {
	version = 1,                            -- before each operation, increment the version number
	runner_name = "StockRunner",
	strategy_name = "slope",
	data_series = "daily,min",
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
	-- Legal Historical data bar size are: 1 secs, 5 secs, 10 secs, 15 secs, 30 secs,
	-- 1 min, 2 mins, 3 mins, 5 mins, 10 mins, 15 mins, 20 mins, 30 mins,
	-- 1 hour, 2 hours, 3 hours, 4 hours, 8 hours, 
	-- 1 day, 1W, 1M
	daily_bar_type = "1 day",
	daily_up_to_date = false,

	min_ticker = "AAPL",
	min_type = "historical",
	min_start_date = "2024-08-01 00:00:00",
	min_end_date = "now",
	min_security_type = "STK",
	min_exchange = "SMART",
	min_currency = "USD",
	min_use_rth = false,
	min_timezone = "America/New_York",
	min_what = "TRADES",
	-- Legal Historical data bar size are: 1 secs, 5 secs, 10 secs, 15 secs, 30 secs,
	-- 1 min, 2 mins, 3 mins, 5 mins, 10 mins, 15 mins, 20 mins, 30 mins,
	-- 1 hour, 2 hours, 3 hours, 4 hours, 8 hours, 
	-- 1 day, 1W, 1M
	min_bar_type = "15 mins",
	min_up_to_date = false,
}


msft_slope = {
	version = 1,                            -- before each operation, increment the version number
	runner_name = "StockRunner",
	strategy_name = "slope",
	symbol = "MSFT",
	start_date = "2016-01-01 00:00:00",
	end_date = "2024-12-31 23:59:59",
	timezone = "America/New_York",
}
