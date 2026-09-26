-- =====================================================================
-- Y2038 Database Migration Suite
-- Migrating 32-bit timestamp storage to 64-bit safe data types
-- =====================================================================

-- ---------------------------------------------------------------------
-- 1. PostgreSQL Schema Migration
-- ---------------------------------------------------------------------

-- Convert legacy INT (32-bit signed) Unix timestamp columns to BIGINT (64-bit)
ALTER TABLE user_sessions 
  ALTER COLUMN expires_at TYPE BIGINT USING expires_at::BIGINT;

ALTER TABLE auth_tokens 
  ALTER COLUMN created_at TYPE BIGINT USING created_at::BIGINT;

-- Preferred alternative: Migrate to PostgreSQL TIMESTAMPTZ (64-bit native timestamp with timezone)
ALTER TABLE audit_logs 
  ALTER COLUMN log_timestamp TYPE TIMESTAMPTZ 
  USING to_timestamp(log_timestamp);


-- ---------------------------------------------------------------------
-- 2. MySQL / MariaDB Schema Migration
-- ---------------------------------------------------------------------

-- Convert legacy INT(11) Unix epoch timestamp columns to BIGINT (64-bit)
ALTER TABLE user_tokens 
  MODIFY COLUMN token_expiry BIGINT NOT NULL;

-- Convert legacy TIMESTAMP (32-bit in older MySQL versions) to DATETIME(6)
ALTER TABLE transactions 
  MODIFY COLUMN created_at DATETIME(6) NOT NULL;


-- ---------------------------------------------------------------------
-- 3. SQLite Schema & Application Binding Guidelines
-- ---------------------------------------------------------------------

-- SQLite uses dynamic typing and stores INTEGER values in 1, 2, 3, 4, 6, or 8 bytes.
-- Ensure tables use INTEGER type for epoch timestamps:
CREATE TABLE IF NOT EXISTS system_events (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    event_name TEXT NOT NULL,
    timestamp_epoch_sec INTEGER NOT NULL -- 64-bit safe epoch timestamp
);

-- Application bindings MUST use sqlite3_bind_int64() instead of sqlite3_bind_int()!
