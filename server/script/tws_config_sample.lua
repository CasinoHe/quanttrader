
tws_provider = {
	host = "127.0.0.1",
	port = 8001,
	clientid = 0,

	retry_interval = 10000,  -- retry interval in milliseconds
	wait_timeout = 10,      -- wait data from TWS timeout in milliseconds, not updated unless restart quanttrader, TODO
        update_config_interval = 60000, -- update config interval in milliseconds
        record_log = false,         -- record log or not
        stop_flag = false,          -- stop flag
        session_start = "09:30:00", -- trading session start time
        session_end = "16:00:00",   -- trading session end time
}