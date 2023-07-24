delete from logsys_categories where log_category_id = 30;
insert into logsys_categories values (30, "Login_Server", 0, 0, 0);
delete from tblloginserversettings where type in ("trace", "world_trace");