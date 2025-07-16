ALTER TABLE player_event_logs ROW_FORMAT=COMPRESSED;
CREATE INDEX idx_event_type_char_id ON player_event_logs (event_type_id, character_id);