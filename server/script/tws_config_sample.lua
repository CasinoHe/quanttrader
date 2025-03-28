
tws_provider = {
	host = "127.0.0.1",
	port = 8001,
	clientid = 0,

	retry_interval = 10000,  -- retry interval in milliseconds
	wait_timeout = 10,      -- wait data from TWS timeout in milliseconds, not updated unless restart quanttrader, TODO
	update_config_interval = 60000, -- update config interval in milliseconds
	record_log = 0,         -- record log or not
	stop_flag = 0,          -- stop flag
}