-- configuration for data provider

daily_data = {
	type = "historical",
	start_date = "2024-08-01 00:00:00",
	end_date = "now",
	security_type = "STK",
	exchange = "SMART",
	currency = "USD",
	use_rth = false,
	timezone = "America/New_York",
	what = "TRADES",
	-- Legal Historical data bar size are: 1 secs, 5 secs, 10 secs, 15 secs, 30 secs,
	-- 1 min, 2 mins, 3 mins, 5 mins, 10 mins, 15 mins, 20 mins, 30 mins,
	-- 1 hour, 2 hours, 3 hours, 4 hours, 8 hours,
	-- 1 day, 1W, 1M
	bar_type = "1 day",
	up_to_date = false,
	resample = false,  -- resample data or not
	resample_bar_size = "1 day",  -- resample bar type
	session_start = "09:30:00",
	session_end = "16:00:00",
}

min_data = {
	type = "historical",
	start_date = "2024-08-01 00:00:00",
	end_date = "now",
	security_type = "STK",
	exchange = "SMART",
	currency = "USD",
	use_rth = false,
	timezone = "America/New_York",
	what = "TRADES",
	-- Legal Historical data bar size are: 1 secs, 5 secs, 10 secs, 15 secs, 30 secs,
	-- 1 min, 2 mins, 3 mins, 5 mins, 10 mins, 15 mins, 20 mins, 30 mins,
	-- 1 hour, 2 hours, 3 hours, 4 hours, 8 hours,
	-- 1 day, 1W, 1M
	bar_type = "15 mins",
	up_to_date = false,
	resample = false,  -- resample data or not
	resample_bar_size = "15 mins",  -- resample bar type
}

sec_data = {
	type = "historical",
	start_date = "2025-04-28 00:00:00",
	end_date = "now",
	security_type = "STK",
	exchange = "SMART",
	currency = "USD",
	use_rth = false,
	timezone = "America/New_York",
	what = "TRADES",
	-- Legal Historical data bar size are: 1 secs, 5 secs, 10 secs, 15 secs, 30 secs,
	-- 1 min, 2 mins, 3 mins, 5 mins, 10 mins, 15 mins, 20 mins, 30 mins,
	-- 1 hour, 2 hours, 3 hours, 4 hours, 8 hours,
	-- 1 day, 1W, 1M
	bar_type = "5 secs",
	up_to_date = false,
	resample = false,  -- resample data or not
	resample_bar_size = "5 secs",  -- resample bar type
}