-- create table
CREATE TABLE accounts(
    uname VARCHAR(50) UNIQUE NOT NULL,
    urole VARCHAR(50) NOT NULL
);
CREATE TABLE admins(
	adname VARCHAR(50) NOT NULL,
	adpassword VARCHAR(50) NOT NULL
);

CREATE TABLE users(
    user_id BIGSERIAL PRIMARY KEY,
    uname VARCHAR(50) UNIQUE NOT NULL,
    upassword VARCHAR(50) DEFAULT '1',
    wallet_id BIGINT REFERENCES wallet(wallet_id)
	--birth_date,
	--CCCD BIGINT UNIQUE,
	--Email VARCHAR(50),
	--address
);

CREATE TABLE totalpoints(
	total BIGINT DEFAULT 50000,
	pointin BIGINT DEFAULT 0,
	pointout BIGINT DEFAULT 0
);

CREATE TABLE Transactioncode(
	wallet_id BIGINT REFERENCES wallet(wallet_id),
	transaction_id BIGSERIAL PRIMARY KEY
);

CREATE TABLE wallet(
	wallet_id BIGSERIAL PRIMARY KEY,
	balance BIGINT NOT NULL
);

CREATE TABLE history(
    transaction_id BIGINT NOT NULL REFERENCES Transactioncode(transaction_id),
	from_user_wallet BIGINT NOT NULL REFERENCES wallet(wallet_id),
    to_user_wallet BIGINT NOT NULL REFERENCES wallet(wallet_id),
    points_used INT NOT NULL,
    balance_before BIGINT NOT NULL,
	balance_after BIGINT NOT NULL,
    date_excute timestamp NOT NULL DEFAULT NOW()::TIMESTAMP(0), -- local time generate when insert into history 
	status VARCHAR(50) NOT NULL
);

-- modify for best performance
CREATE INDEX wallet_id_index ON Transacioncode(wallet_id);
CREATE INDEX transaction_id_index ON history(transaction_id);
CREATE INDEX uname_index ON users(uname);
CREATE INDEX accname_index ON accounts(uname);


-- don't know maybe delete
SELECT urole FROM users WHERE uname = '' /*check uname exist in SQL when logging*/
SELECT * FROM users LIMIT 5 ORDER BY ASC; /*print 5 roll from beginning of users table*/ 


INSERT INTO users(uname, urole) values ('', ''); /*add user or admin to control in system */
INSERT INTO userwithwallet(uname, upassword, upoints) values ();/* add user info */
INSERT INTO history(user_id, from_user, to_user, points_transferred, ubalance) values(); /*add history for tracking*/
UPDATE userwithwallet SET ubalance = ubalance - history.points_transeferred WHERE userwithwallet.user_id = history.user_id; /*update sender's balance*/
UPDATE userwithwallet SET ubalance = ubalance + history.points_transeferred WHERE userwithwallet.uname = history.to_user; /* update receiver's balance */


-- update pointout for each user created
INSERT INTO users(uname, upassword, ) VALUES ();
UPDATE totalpoints SET pointout = pointout + 5000;


--check log in
CREATE OR REPLACE FUNCTION login(name_input VARCHAR(50))
RETURNS TABLE(name VARCHAR(50), upassword VARCHAR(50), wallet_id BIGINT, urole VARCHAR(50))
LANGUAGE plpgsql AS $$
DECLARE
    user_role VARCHAR(50);  -- Đổi tên biến để tránh trùng với tên cột
BEGIN
    -- Kiểm tra xem có bản ghi nào khớp với name_input không
    IF NOT EXISTS (
        SELECT 1
        FROM accounts AS a
        WHERE a.uname = name_input
    ) THEN
        -- Nếu không có bản ghi nào khớp, trả về "0" cho tất cả các cột
        RETURN QUERY SELECT '0'::VARCHAR, '0'::VARCHAR, 0::BIGINT, '0'::VARCHAR;
    ELSE
        -- Nếu tìm thấy bản ghi, lấy vai trò của người dùng
        SELECT a.urole INTO user_role FROM accounts AS a WHERE a.uname = name_input;

        -- Kiểm tra vai trò của người dùng
        IF user_role = 'admin' THEN
            RETURN QUERY SELECT name_input, '0'::VARCHAR, 0::BIGINT, user_role;
        ELSE
            RETURN QUERY 
            SELECT u.uname, u.upassword, u.wallet_id, user_role
            FROM users AS u
            WHERE u.uname = name_input;
        END IF;
    END IF;
END;
$$;



-- update user info
CREATE OR REPLACE update_uinfo()
RETURN TEXT AS $$
BEGIN
	UPDATE users SET upassword = new_upassword;
	UPDATE .......

	RETURN 'Cập nhật thành công';
END;
EXCEPTION
    -- Catch any errors and return a failure message
    WHEN OTHERS THEN
        RETURN format('Cập nhật thất bại: %s', SQLERRM);

-- show list of user




-- show transfer log
CREATE OR REPLACE FUNCTION transfer_log(p_wallet_id BIGINT, track_number BIGINT)
RETURNS TABLE(
    "Ngày thực hiện" TIMESTAMP, 
    "Điểm sử dụng" INT, 
    "Số dư trước giao dịch" BIGINT, 
    "Số dư sau giao dịch" BIGINT, 
    "Trạng thái giao dịch" VARCHAR
) AS $$
BEGIN
    RETURN QUERY
    SELECT 
        h.date_excute AS "Ngày thực hiện", 
        h.points_used AS "Điểm sử dụng", 
        h.balance_before AS "Số dư trước giao dịch", 
        h.balance_after AS "Số dư sau giao dịch", 
        h.status AS "Trạng thái giao dịch"
    FROM history h
    INNER JOIN Transactioncode t ON h.transaction_id = t.transaction_id
    WHERE t.wallet_id = p_wallet_id
	ORDER BY h.date_excute DESC
	LIMIT 5
	OFFSET track_number;
END;
$$ LANGUAGE plpgsql;






-- check pointout and pointin when closing system
CREATE OR REPLACE FUNCTION check_total()
RETURNS TEXT AS $$
DECLARE
	total BIGINT;
	pin BIGINT;
	pout BIGINT;
BEGIN
	SELECT pointin INTO pin FROM totalpoints;
	SELECT pointout INTO pout FROM totalpoints;
	IF pin != pout THEN
		BEGIN
			SELECT sum(balance) INTO total FROM wallet;
			pin := pin + total;
			UPDATE wallet SET balance = 0;
			UPDATE totalpoints SET pointin = pin;
		END;
	END IF;
	
	RETURN format('Chuong trinh ket thuc');
	
EXCEPTION
   	WHEN OTHERS THEN
    	-- Catch any errors and return a failure message
        RETURN format('Đã xảy ra lỗi', SQLERRM);
END;
$$ LANGUAGE plpgsql;




-- check username exist in SQL for logging or transfer
CREATE OR REPLACE FUNCTION check_user(name_input VARCHAR(50))
RETURNS TABLE(name VARCHAR(50), wallet_id BIGINT) AS $$
BEGIN
    -- Thử tìm tên và wallet_id của người dùng
    RETURN QUERY
    SELECT u.uname, u.wallet_id
    FROM users u
    WHERE u.uname = name_input;

    -- Nếu không tìm thấy, trả về mặc định '0' và 0
    IF NOT FOUND THEN
        RETURN QUERY SELECT '0'::VARCHAR, 0::BIGINT;
    END IF;
END;
$$ LANGUAGE plpgsql;




-- create user account
CREATE OR REPLACE FUNCTION create_user(uname VARCHAR(50), upassword VARCHAR(50) DEFAULT '1', value NUMERIC DEFAULT 5000)
RETURNS TEXT AS $$
DECLARE
    uwallet_id BIGINT;
BEGIN
    -- Insert a new wallet with the specified balance
    INSERT INTO wallet(balance) VALUES(value)
    RETURNING wallet_id INTO uwallet_id;

    -- Insert a new user with the generated wallet_id and update pointout
    INSERT INTO users(uname, upassword, wallet_id) VALUES(uname, upassword, uwallet_id);
	UPDATE totalpoints SET pointout = pointout + 5000;

    -- Return success message
    RETURN 'Tạo tài khoản thành công';

EXCEPTION
    -- Catch any errors and return a failure message
    WHEN OTHERS THEN
        RETURN format('Tạo tài khoản thất bại: %s', SQLERRM);
END;
$$ LANGUAGE plpgsql;



-- transfer and catch exceptions
CREATE OR REPLACE FUNCTION complex_transaction(A BIGINT, B BIGINT, d BIGINT)
RETURNS TEXT AS $$
DECLARE
    balance_A BIGINT;
    balance_B BIGINT;
    Tcode_id BIGINT;
    txn_status VARCHAR(50);
BEGIN
    -- Start transaction block
    BEGIN
        -- Get the balances of wallet A and B
        SELECT balance INTO balance_A FROM wallet WHERE wallet_id = A;
        SELECT balance INTO balance_B FROM wallet WHERE wallet_id = B;

        -- Always generate a transaction_id regardless of success or failure
        INSERT INTO Transactioncode(wallet_id)
        VALUES (A)
        RETURNING transaction_id INTO Tcode_id;

        -- Check if balance of A is sufficient for the transaction
        IF balance_A >= d THEN
            -- Update the balances for wallet A and B
            UPDATE wallet SET balance = balance - d WHERE wallet_id = A;
            UPDATE wallet SET balance = balance + d WHERE wallet_id = B;

            -- Mark the transaction as successful
            txn_status := 'Success';
        ELSE
            -- Mark the transaction as failed due to insufficient balance
            txn_status := 'Failed';
        END IF;

        -- Insert into history table for tracking
        INSERT INTO history(
            transaction_id, from_user_wallet, to_user_wallet, points_used, balance_before, balance_after, status
        )
        VALUES (
            Tcode_id, 
            A, 
            B, 
            d, 
            balance_A,  -- Balance before the transaction
            CASE WHEN txn_status = 'Success' THEN balance_A - d ELSE balance_A END,  -- Balance after transaction or unchanged
            txn_status
        );

        -- Return the appropriate message based on success or failure
        IF txn_status = 'Success' THEN
            RETURN format('Giao dịch thành công.');
        ELSE
            RETURN 'Số dư tài khoản của bạn không đủ để thực hiện giao dịch.';
        END IF;

    -- Exception handling block
    EXCEPTION
        WHEN OTHERS THEN
            -- Rollback and catch any errors and return a failure message
            RETURN format('Giao dịch bị hủy do lỗi: %s', SQLERRM);
    END;
END;
$$ LANGUAGE plpgsql;



-- show data in table
SELECT * FROM accounts;
SELECT * FROM admins;
SELECT * FROM users;
SELECT * FROM Transactioncode;
SELECT * FROM history;
SELECT * FROM totalpoints;
SELECT * FROM wallet;

-- delete table
DROP TABLE users;
DROP TABLE Transactioncode;
DROP TABLE history;
DROP TABLE totalpoints;
DROP TABLE wallet;
DROP FUNCTION complex_transaction(A BIGINT, B BIGINT, d BIGINT);
DROP FUNCTION check_user(name VARCHAR(50));
DROP FUNCTION transfer_log(wallet_id BIGINT);
DROP FUNCTION login(name_input VARCHAR(50));


-- clear data in table
TRUNCATE history;
TRUNCATE users;
TRUNCATE Transactioncode;


-- test case
INSERT INTO admins(adname, adpassword) VALUES ('1', '321');
INSERT INTO accounts(uname, urole) VALUES ('1', 'admin');
INSERT INTO accounts(uname, urole) VALUES ('Quan', 'user');
INSERT INTO accounts(uname, urole) VALUES ('Huy', 'user');
SELECT create_user('Quan');
UPDATE totalpoints SET pointout = pointout + 5000;
SELECT create_user('Huy')
UPDATE totalpoints SET pointout = pointout + 5000;
INSERT INTO totalpoints(total) VALUES (50000);
SELECT complex_transaction(1, 2, 50000);
SELECT complex_transaction(1, 2, 2000);
SELECT * FROM check_user('Long');
SELECT * FROM check_user('Huy');
SELECT * FROM transfer_log(1, 0);
SELECT * FROM login('Long');
SELECT * FROM login('1');
SELECT check_total();


