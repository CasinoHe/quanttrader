-- stock trade service configuration
StockTradeService = {
	-- settings for broker
	broker_provider = "tws",
	broker_config = "tws_config.lua",  -- "this" means we should load config from this file

	-- settings for data series
	data_series = "daily_data,min_data,sec_data",
	daily_data = {
		provider_type = "tws",
		provider_config = "data_config.lua",  -- configuration for daily data provider
		tickers = "AAPL,MSFT,GOOG",
	},
	min_data = {
		provider_type = "tws",
		provider_config = "data_config.lua",  -- configuration for min data provider
		tickers = "AAPL,MSFT,GOOG",
	},
	sec_data = {
		provider_type = "tws",
		provider_config = "data_config.lua",  -- configuration for sec data provider
		tickers = "AAPL,MSFT,GOOG",
	},

	-- setting for cerebro
	cerebro_names = "backtest_cerebro,live_cerebro",
	backtest_cerebro = {
		cerebro_type = "",
		cerebro_config = "this",
	},
	live_cerebro = {
		cerebro_type = "",
		cerebro_config = "this",
	},
	risk_manager = {
		"to_be_implemented",
		"to_be_implemented2",
	},
	engine = "fortune",  -- only forturn for now
	mode = "backtest",  -- backtest or live
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
