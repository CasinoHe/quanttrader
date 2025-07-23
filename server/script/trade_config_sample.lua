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

        -- settings for strategy
        strategy_lib_path = "strategies",
        strategy_names = "apple_slope,msft_slope",

	-- setting for cerebro
	cerebro_names = "backtest_cerebro,live_cerebro",
	backtest_cerebro = {
		cerebro_type = "backtest",
		replay_mode = "normal",
		wait_data_timeout = 120000, -- wait data from data provider timeout in milliseconds, not updated unless restart quanttrader
	},
	live_cerebro = {
		cerebro_type = "live",
		replay_mode = "normal",
		wait_data_timeout = 120000, -- wait data from data provider timeout in milliseconds, not updated unless restart quanttrader
	},
	risk_manager = {
		"to_be_implemented",
		"to_be_implemented2",
	},
	engine = "fortune",  -- only forturn for now
	mode = "backtest",  -- backtest or live
}

apple_slope = {
	strategy_type = "slope",
	version = 1,                            -- before each operation, increment the version number
	symbol = "AAPL",
	data_series = "daily,min",
	log_data = true,  -- key data debug switch
}


msft_slope = {
	strategy_type = "slope",
	version = 1,                            -- before each operation, increment the version number
	symbol = "MSFT",
	start_date = "2016-01-01 00:00:00",
	end_date = "2024-12-31 23:59:59",
	timezone = "America/New_York",
	log_data = false,  -- key data debug switch
}
