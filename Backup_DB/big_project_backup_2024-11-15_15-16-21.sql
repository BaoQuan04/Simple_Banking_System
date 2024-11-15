PGDMP                  
    |            big_project    16.0    16.0 9    .           0    0    ENCODING    ENCODING        SET client_encoding = 'UTF8';
                      false            /           0    0 
   STDSTRINGS 
   STDSTRINGS     (   SET standard_conforming_strings = 'on';
                      false            0           0    0 
   SEARCHPATH 
   SEARCHPATH     8   SELECT pg_catalog.set_config('search_path', '', false);
                      false            1           1262    33363    big_project    DATABASE     �   CREATE DATABASE big_project WITH TEMPLATE = template0 ENCODING = 'UTF8' LOCALE_PROVIDER = libc LOCALE = 'English_United States.1252';
    DROP DATABASE big_project;
                postgres    false            �            1255    33984    check_user(character varying)    FUNCTION     �  CREATE FUNCTION public.check_user(telephone_input character varying) RETURNS TABLE(utelephone character varying, wallet_id bigint)
    LANGUAGE plpgsql
    AS $$
BEGIN
    -- Thử tìm tên và wallet_id của người dùng
    RETURN QUERY
    SELECT u.utelephone, u.wallet_id
    FROM users u
    WHERE u.utelephone = telephone_input;

    -- Nếu không tìm thấy, trả về mặc định '0' và 0
    IF NOT FOUND THEN
        RETURN QUERY SELECT '0'::VARCHAR, 0::BIGINT;
    END IF;
END;
$$;
 D   DROP FUNCTION public.check_user(telephone_input character varying);
       public          postgres    false            �            1255    34820    check_user_balance(bigint)    FUNCTION     �   CREATE FUNCTION public.check_user_balance(input_wallet_id bigint) RETURNS TABLE(ubalance bigint)
    LANGUAGE plpgsql
    AS $$
BEGIN
	RETURN QUERY
	SELECT balance FROM wallet WHERE wallet.wallet_id = input_wallet_id;
END;
$$;
 A   DROP FUNCTION public.check_user_balance(input_wallet_id bigint);
       public          postgres    false            �            1255    33987    check_wallet_id(bigint)    FUNCTION       CREATE FUNCTION public.check_wallet_id(input_wallet bigint) RETURNS TABLE(wallet_id bigint)
    LANGUAGE plpgsql
    AS $$
BEGIN
	RETURN QUERY
	SELECT u.wallet_id 
	FROM users u
	WHERE u.wallet_id = input_wallet;
	
	IF NOT FOUND THEN
		RETURN QUERY SELECT 0::BIGINT;
	END IF;
END;
$$;
 ;   DROP FUNCTION public.check_wallet_id(input_wallet bigint);
       public          postgres    false            �            1255    34821 +   complex_transaction(bigint, bigint, bigint)    FUNCTION     d
  CREATE FUNCTION public.complex_transaction(a bigint, b bigint, d bigint) RETURNS text
    LANGUAGE plpgsql
    AS $$
DECLARE
    balance_A BIGINT;
    balance_B BIGINT;
    Tcode_id BIGINT;
BEGIN
    
		-- Kiểm tra số dư trước khi giao dịch
        SELECT balance INTO balance_A FROM wallet WHERE wallet_id = A;
     
		
		-- Lấy số dư ví của B
       	SELECT balance INTO balance_B FROM wallet WHERE wallet_id = B;
		
        -- Tạo transaction_id 
		INSERT INTO Transactioncode(from_wallet_id, to_wallet_id)
        VALUES (A, B)
        RETURNING transaction_id INTO Tcode_id;
			
		-- bắt đầu giao dịch
		BEGIN
			
            -- Cập nhật số dư cho ví A và B
            UPDATE wallet SET balance = balance - d WHERE wallet_id = A;
            UPDATE wallet SET balance = balance + d WHERE wallet_id = B;
			
			
			-- Ghi vào history
			INSERT INTO history(
            transaction_id, from_user_wallet, to_user_wallet, points_used, A_balance_before, A_balance_after, B_balance_before, B_balance_after, status
        	)
        	VALUES (
            	Tcode_id, 
            	A, 
            	B, 
            	d, 
            	balance_A,  -- Số dư của A trước khi giao dịch
            	balance_A - d,  -- Số dư của A sau khi giao dịch thành công
            	balance_B,  -- Số dư ban đầu của B
            	balance_B + d, -- Số dư của B sau khi giao dịch thành công
            	'Success'
        	);
			RETURN format('Giao dich thanh cong.');
           

    -- Khối xử lý ngoại lệ
    EXCEPTION
        WHEN OTHERS THEN
			-- Nếu gặp lỗi vẫn sẽ khi vào trong history và transaction
			BEGIN
				INSERT INTO Transactioncode(from_wallet_id, to_wallet_id)
        		VALUES (A, B)
        		RETURNING transaction_id INTO Tcode_id;
				INSERT INTO history(
            	transaction_id, from_user_wallet, to_user_wallet, points_used, A_balance_before, A_balance_after, B_balance_before, B_balance_after, status
        		)
        		VALUES (
            		Tcode_id, 
            		A, 
            		B, 
            		d, 
            		balance_A,  -- Số dư ban đầu của A
            		balance_A,  -- Số dư của A sau khi giao dịch thất bại
            		balance_B,  -- Số dư ban đầu của B
            		balance_B, 	-- Số dư của B sau khi giao dịch thất bại
            		'Failed'
        		);
				EXCEPTION
                WHEN OTHERS THEN
                    -- Nếu gặp lỗi thì dừng lại, không in ra lỗi
                    NULL;
            END;
            RETURN format('Giao dich bi huy !');
		END;
END;
$$;
 H   DROP FUNCTION public.complex_transaction(a bigint, b bigint, d bigint);
       public          postgres    false            �            1255    34589 0   create_user(character varying, numeric, numeric)    FUNCTION     �  CREATE FUNCTION public.create_user(input_telephone character varying, upassword numeric, value numeric DEFAULT 5000) RETURNS text
    LANGUAGE plpgsql
    AS $$
DECLARE
    uwallet_id BIGINT;
BEGIN
    -- Insert a new wallet with the specified balance
    INSERT INTO wallet(balance) VALUES(value)
    RETURNING wallet_id INTO uwallet_id;
	
	INSERT INTO accounts(acctelephone, urole) VALUES (input_telephone, 'user');

    -- Insert a new user with the generated wallet_id and update pointout
    INSERT INTO users(utelephone, upassword, wallet_id) VALUES(input_telephone, upassword, uwallet_id);
	UPDATE totalpoints SET pointout = pointout + 5000;
	
	-- identify first login
	UPDATE users SET first_login = 1 WHERE users.utelephone = input_telephone;
    -- Return success message
    RETURN 'Tao tai khoan thanh cong';

EXCEPTION
    -- Catch any errors and return a failure message
    WHEN OTHERS THEN
        RETURN format('Da xay ra loi vui long thu lai: %s', SQLERRM);
END;
$$;
 g   DROP FUNCTION public.create_user(input_telephone character varying, upassword numeric, value numeric);
       public          postgres    false            �            1255    34744    login(character varying)    FUNCTION     �  CREATE FUNCTION public.login(telephone_input character varying) RETURNS TABLE(utelephone character varying, uname character varying, upassword numeric, wallet_id bigint, urole character varying)
    LANGUAGE plpgsql
    AS $$
DECLARE
    user_role VARCHAR(50);  -- Biến lưu vai trò của người dùng
BEGIN
    -- Kiểm tra xem có bản ghi nào trong bảng accounts khớp với name_input không
    IF NOT EXISTS (
        SELECT 1
        FROM accounts AS a
        WHERE a.acctelephone = telephone_input
    ) THEN
        -- Nếu không có bản ghi nào khớp, trả về "0" cho tất cả các cột
        RETURN QUERY SELECT '0'::VARCHAR(50), '0'::VARCHAR(50), 0::NUMERIC, 0::BIGINT, '0'::VARCHAR(50);
    ELSE
        -- Nếu tìm thấy bản ghi, lấy vai trò của người dùng từ bảng accounts
        SELECT a.urole INTO user_role FROM accounts AS a WHERE a.acctelephone = telephone_input;

        -- Dựa vào vai trò người dùng, trả về dữ liệu từ bảng admin hoặc users
        IF user_role = 'admin' THEN
            RETURN QUERY 
            SELECT ad.adname, '0'::VARCHAR(50), ad.adpassword, 0::BIGINT, user_role
            FROM admins AS ad
            WHERE ad.adname = telephone_input;
        ELSE
            RETURN QUERY 
            SELECT u.utelephone, u.uname, u.upassword, u.wallet_id, user_role
            FROM users AS u
            WHERE u.utelephone = telephone_input;
        END IF;
    END IF;
END;
$$;
 ?   DROP FUNCTION public.login(telephone_input character varying);
       public          postgres    false            �            1255    34374    transfer_log(bigint, bigint)    FUNCTION     F  CREATE FUNCTION public.transfer_log(p_wallet_id bigint, track_number bigint) RETURNS TABLE("Ngay thuc hien" timestamp without time zone, "Noi dung" text, "Diem su dung" integer, "So du truoc giao dich" bigint, "So du sau giao dich" bigint, "Trang thai giao dich" text)
    LANGUAGE plpgsql
    AS $$
BEGIN
    RETURN QUERY
    SELECT 
        h.date_excute AS "Ngay thuc hien",
        
        -- Xác định nội dung dựa trên cột `from_user_wallet` hoặc `to_user_wallet`
        CASE 
            WHEN h.from_user_wallet = p_wallet_id THEN 'Chuyen diem den ' || COALESCE(u_to.utelephone, 'Không rõ')
            ELSE 'Nhan diem tu ' || COALESCE(u_from.utelephone, 'Khong ro')
        END AS "Noi dung",
        
        h.points_used AS "Diem su dung",

        -- Xác định "So du truoc giao dich" dựa trên nội dung
        CASE 
            WHEN h.from_user_wallet = p_wallet_id THEN h.A_balance_before
            ELSE h.B_balance_before
        END AS "So du truoc giao dich",
        
        -- Xác định "So du sau giao dich" dựa trên nội dung
        CASE 
            WHEN h.from_user_wallet = p_wallet_id THEN h.A_balance_after
            ELSE h.B_balance_after
        END AS "So du sau giao dich",

        h.status::TEXT AS "Trang thai giao dich"
    FROM history h
    INNER JOIN Transactioncode t ON h.transaction_id = t.transaction_id
    LEFT JOIN users u_from ON h.from_user_wallet = u_from.wallet_id
    LEFT JOIN users u_to ON h.to_user_wallet = u_to.wallet_id
    WHERE 
        (h.from_user_wallet = p_wallet_id)  -- Luôn hiển thị nếu người dùng là người gửi
        OR (h.to_user_wallet = p_wallet_id AND h.status = 'Success')  -- Hiển thị khi là người nhận và trạng thái giao dịch 'thành công'
    ORDER BY h.date_excute DESC
    LIMIT 5
    OFFSET track_number;
END;
$$;
 L   DROP FUNCTION public.transfer_log(p_wallet_id bigint, track_number bigint);
       public          postgres    false            �            1255    34270 i   update_uinfo(bigint, character varying, numeric, character varying, character varying, character varying)    FUNCTION     �  CREATE FUNCTION public.update_uinfo(input_wallet_id bigint, new_uname character varying DEFAULT NULL::character varying, new_upassword numeric DEFAULT NULL::numeric, new_birth character varying DEFAULT NULL::character varying, new_email character varying DEFAULT NULL::character varying, new_sex character varying DEFAULT NULL::character varying) RETURNS text
    LANGUAGE plpgsql
    AS $$
BEGIN
    -- Cập nhật trực tiếp với các giá trị không phải NULL
    UPDATE users
    SET 
        uname = COALESCE(new_uname, uname),  -- Nếu new_uname không NULL, cập nhật; ngược lại giữ nguyên
        upassword = COALESCE(new_upassword, upassword),
        birth_date = COALESCE(new_birth, birth_date),
        email = COALESCE(new_email, email),
        sex = COALESCE(new_sex, sex)
    WHERE wallet_id = input_wallet_id;

    RETURN 'Cap nhat thanh cong';
EXCEPTION
    WHEN OTHERS THEN
        RETURN format('Cap nhat that bai: %s', SQLERRM);
END;
$$;
 �   DROP FUNCTION public.update_uinfo(input_wallet_id bigint, new_uname character varying, new_upassword numeric, new_birth character varying, new_email character varying, new_sex character varying);
       public          postgres    false            �            1259    34824    accounts    TABLE     |   CREATE TABLE public.accounts (
    acctelephone character varying(50) NOT NULL,
    urole character varying(50) NOT NULL
);
    DROP TABLE public.accounts;
       public         heap    postgres    false            �            1259    34829    admins    TABLE     k   CREATE TABLE public.admins (
    adname character varying(50) NOT NULL,
    adpassword numeric NOT NULL
);
    DROP TABLE public.admins;
       public         heap    postgres    false            �            1259    34877    history    TABLE     �  CREATE TABLE public.history (
    transaction_id bigint NOT NULL,
    from_user_wallet bigint NOT NULL,
    to_user_wallet bigint NOT NULL,
    points_used integer NOT NULL,
    a_balance_before bigint NOT NULL,
    a_balance_after bigint NOT NULL,
    b_balance_before bigint NOT NULL,
    b_balance_after bigint NOT NULL,
    date_excute timestamp without time zone DEFAULT (now())::timestamp(0) without time zone NOT NULL,
    status character varying(50) NOT NULL
);
    DROP TABLE public.history;
       public         heap    postgres    false            �            1259    34605    totalpoints    TABLE     �   CREATE TABLE public.totalpoints (
    total bigint DEFAULT 50000,
    pointin bigint DEFAULT 0,
    pointout bigint DEFAULT 0
);
    DROP TABLE public.totalpoints;
       public         heap    postgres    false            �            1259    34861    transactioncode    TABLE     �   CREATE TABLE public.transactioncode (
    from_wallet_id bigint,
    to_wallet_id bigint,
    transaction_id bigint NOT NULL
);
 #   DROP TABLE public.transactioncode;
       public         heap    postgres    false            �            1259    34860 "   transactioncode_transaction_id_seq    SEQUENCE     �   CREATE SEQUENCE public.transactioncode_transaction_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 9   DROP SEQUENCE public.transactioncode_transaction_id_seq;
       public          postgres    false    223            2           0    0 "   transactioncode_transaction_id_seq    SEQUENCE OWNED BY     i   ALTER SEQUENCE public.transactioncode_transaction_id_seq OWNED BY public.transactioncode.transaction_id;
          public          postgres    false    222            �            1259    34842    users    TABLE     J  CREATE TABLE public.users (
    user_id bigint NOT NULL,
    utelephone character varying(50) NOT NULL,
    uname character varying(50),
    upassword numeric NOT NULL,
    birth_date character varying(50),
    email character varying(50),
    sex character varying(50),
    wallet_id bigint,
    first_login numeric DEFAULT 0
);
    DROP TABLE public.users;
       public         heap    postgres    false            �            1259    34841    users_user_id_seq    SEQUENCE     z   CREATE SEQUENCE public.users_user_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 (   DROP SEQUENCE public.users_user_id_seq;
       public          postgres    false    221            3           0    0    users_user_id_seq    SEQUENCE OWNED BY     G   ALTER SEQUENCE public.users_user_id_seq OWNED BY public.users.user_id;
          public          postgres    false    220            �            1259    34835    wallet    TABLE     [   CREATE TABLE public.wallet (
    wallet_id bigint NOT NULL,
    balance bigint NOT NULL
);
    DROP TABLE public.wallet;
       public         heap    postgres    false            �            1259    34834    wallet_wallet_id_seq    SEQUENCE     }   CREATE SEQUENCE public.wallet_wallet_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 +   DROP SEQUENCE public.wallet_wallet_id_seq;
       public          postgres    false    219            4           0    0    wallet_wallet_id_seq    SEQUENCE OWNED BY     M   ALTER SEQUENCE public.wallet_wallet_id_seq OWNED BY public.wallet.wallet_id;
          public          postgres    false    218            x           2604    34864    transactioncode transaction_id    DEFAULT     �   ALTER TABLE ONLY public.transactioncode ALTER COLUMN transaction_id SET DEFAULT nextval('public.transactioncode_transaction_id_seq'::regclass);
 M   ALTER TABLE public.transactioncode ALTER COLUMN transaction_id DROP DEFAULT;
       public          postgres    false    222    223    223            v           2604    34845    users user_id    DEFAULT     n   ALTER TABLE ONLY public.users ALTER COLUMN user_id SET DEFAULT nextval('public.users_user_id_seq'::regclass);
 <   ALTER TABLE public.users ALTER COLUMN user_id DROP DEFAULT;
       public          postgres    false    221    220    221            u           2604    34838    wallet wallet_id    DEFAULT     t   ALTER TABLE ONLY public.wallet ALTER COLUMN wallet_id SET DEFAULT nextval('public.wallet_wallet_id_seq'::regclass);
 ?   ALTER TABLE public.wallet ALTER COLUMN wallet_id DROP DEFAULT;
       public          postgres    false    218    219    219            #          0    34824    accounts 
   TABLE DATA           7   COPY public.accounts (acctelephone, urole) FROM stdin;
    public          postgres    false    216   �d       $          0    34829    admins 
   TABLE DATA           4   COPY public.admins (adname, adpassword) FROM stdin;
    public          postgres    false    217   $e       +          0    34877    history 
   TABLE DATA           �   COPY public.history (transaction_id, from_user_wallet, to_user_wallet, points_used, a_balance_before, a_balance_after, b_balance_before, b_balance_after, date_excute, status) FROM stdin;
    public          postgres    false    224   Ke       "          0    34605    totalpoints 
   TABLE DATA           ?   COPY public.totalpoints (total, pointin, pointout) FROM stdin;
    public          postgres    false    215   �e       *          0    34861    transactioncode 
   TABLE DATA           W   COPY public.transactioncode (from_wallet_id, to_wallet_id, transaction_id) FROM stdin;
    public          postgres    false    223   f       (          0    34842    users 
   TABLE DATA           v   COPY public.users (user_id, utelephone, uname, upassword, birth_date, email, sex, wallet_id, first_login) FROM stdin;
    public          postgres    false    221   Ff       &          0    34835    wallet 
   TABLE DATA           4   COPY public.wallet (wallet_id, balance) FROM stdin;
    public          postgres    false    219   /g       5           0    0 "   transactioncode_transaction_id_seq    SEQUENCE SET     P   SELECT pg_catalog.setval('public.transactioncode_transaction_id_seq', 7, true);
          public          postgres    false    222            6           0    0    users_user_id_seq    SEQUENCE SET     ?   SELECT pg_catalog.setval('public.users_user_id_seq', 9, true);
          public          postgres    false    220            7           0    0    wallet_wallet_id_seq    SEQUENCE SET     C   SELECT pg_catalog.setval('public.wallet_wallet_id_seq', 10, true);
          public          postgres    false    218            |           2606    34828 "   accounts accounts_acctelephone_key 
   CONSTRAINT     e   ALTER TABLE ONLY public.accounts
    ADD CONSTRAINT accounts_acctelephone_key UNIQUE (acctelephone);
 L   ALTER TABLE ONLY public.accounts DROP CONSTRAINT accounts_acctelephone_key;
       public            postgres    false    216            �           2606    34866 $   transactioncode transactioncode_pkey 
   CONSTRAINT     n   ALTER TABLE ONLY public.transactioncode
    ADD CONSTRAINT transactioncode_pkey PRIMARY KEY (transaction_id);
 N   ALTER TABLE ONLY public.transactioncode DROP CONSTRAINT transactioncode_pkey;
       public            postgres    false    223            �           2606    34850    users users_pkey 
   CONSTRAINT     S   ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_pkey PRIMARY KEY (user_id);
 :   ALTER TABLE ONLY public.users DROP CONSTRAINT users_pkey;
       public            postgres    false    221            �           2606    34854    users users_uname_key 
   CONSTRAINT     Q   ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_uname_key UNIQUE (uname);
 ?   ALTER TABLE ONLY public.users DROP CONSTRAINT users_uname_key;
       public            postgres    false    221            �           2606    34852    users users_utelephone_key 
   CONSTRAINT     [   ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_utelephone_key UNIQUE (utelephone);
 D   ALTER TABLE ONLY public.users DROP CONSTRAINT users_utelephone_key;
       public            postgres    false    221            ~           2606    34840    wallet wallet_pkey 
   CONSTRAINT     W   ALTER TABLE ONLY public.wallet
    ADD CONSTRAINT wallet_pkey PRIMARY KEY (wallet_id);
 <   ALTER TABLE ONLY public.wallet DROP CONSTRAINT wallet_pkey;
       public            postgres    false    219            z           1259    34900    accname_index    INDEX     J   CREATE INDEX accname_index ON public.accounts USING btree (acctelephone);
 !   DROP INDEX public.accname_index;
       public            postgres    false    216            �           1259    34896    from_wallet_id_index    INDEX     Z   CREATE INDEX from_wallet_id_index ON public.transactioncode USING btree (from_wallet_id);
 (   DROP INDEX public.from_wallet_id_index;
       public            postgres    false    223            �           1259    34902    ht_transaction_id_index    INDEX     U   CREATE INDEX ht_transaction_id_index ON public.history USING btree (transaction_id);
 +   DROP INDEX public.ht_transaction_id_index;
       public            postgres    false    224            �           1259    34897    to_wallet_id_index    INDEX     V   CREATE INDEX to_wallet_id_index ON public.transactioncode USING btree (to_wallet_id);
 &   DROP INDEX public.to_wallet_id_index;
       public            postgres    false    223            �           1259    34898    tr_transaction_id_index    INDEX     ]   CREATE INDEX tr_transaction_id_index ON public.transactioncode USING btree (transaction_id);
 +   DROP INDEX public.tr_transaction_id_index;
       public            postgres    false    223            �           1259    34899    utelephone_index    INDEX     H   CREATE INDEX utelephone_index ON public.users USING btree (utelephone);
 $   DROP INDEX public.utelephone_index;
       public            postgres    false    221            �           1259    34901    uwallet_id_index    INDEX     G   CREATE INDEX uwallet_id_index ON public.users USING btree (wallet_id);
 $   DROP INDEX public.uwallet_id_index;
       public            postgres    false    221            �           2606    34886 %   history history_from_user_wallet_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.history
    ADD CONSTRAINT history_from_user_wallet_fkey FOREIGN KEY (from_user_wallet) REFERENCES public.wallet(wallet_id);
 O   ALTER TABLE ONLY public.history DROP CONSTRAINT history_from_user_wallet_fkey;
       public          postgres    false    219    4734    224            �           2606    34891 #   history history_to_user_wallet_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.history
    ADD CONSTRAINT history_to_user_wallet_fkey FOREIGN KEY (to_user_wallet) REFERENCES public.wallet(wallet_id);
 M   ALTER TABLE ONLY public.history DROP CONSTRAINT history_to_user_wallet_fkey;
       public          postgres    false    4734    224    219            �           2606    34881 #   history history_transaction_id_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.history
    ADD CONSTRAINT history_transaction_id_fkey FOREIGN KEY (transaction_id) REFERENCES public.transactioncode(transaction_id);
 M   ALTER TABLE ONLY public.history DROP CONSTRAINT history_transaction_id_fkey;
       public          postgres    false    223    4747    224            �           2606    34867 3   transactioncode transactioncode_from_wallet_id_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.transactioncode
    ADD CONSTRAINT transactioncode_from_wallet_id_fkey FOREIGN KEY (from_wallet_id) REFERENCES public.wallet(wallet_id);
 ]   ALTER TABLE ONLY public.transactioncode DROP CONSTRAINT transactioncode_from_wallet_id_fkey;
       public          postgres    false    223    4734    219            �           2606    34872 1   transactioncode transactioncode_to_wallet_id_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.transactioncode
    ADD CONSTRAINT transactioncode_to_wallet_id_fkey FOREIGN KEY (to_wallet_id) REFERENCES public.wallet(wallet_id);
 [   ALTER TABLE ONLY public.transactioncode DROP CONSTRAINT transactioncode_to_wallet_id_fkey;
       public          postgres    false    223    4734    219            �           2606    34855    users users_wallet_id_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_wallet_id_fkey FOREIGN KEY (wallet_id) REFERENCES public.wallet(wallet_id);
 D   ALTER TABLE ONLY public.users DROP CONSTRAINT users_wallet_id_fkey;
       public          postgres    false    219    4734    221            #   S   x�5�;�0��9Ji�c��T����ޟ���O��5f�sOQW��[X���&g�+���ͻ�'�18�\��������x'?      $      x�,M��42�0����� �!      +   �   x�u�;�0��9�3I}V��T�B�[��X�϶L @��֋DW{ad�D���qqY��\�Ƕ%~C�l	Z�T�k*B.&�v�*�Rr jC�[M\jX�<r��k�1i���a���o��9a���{V�~�̻����RzoP      "      x������ � �      *   *   x�3�4�4�2�4�4�2����	X�L�I#Ns�=... �H�      (   �   x�]��j�0Dϳ_�/�weɶnm顇b(��rMq�	>��;Q"H:��Xތ�F�ǀa\�?s�yLs����c����V��j �4�qPWD����*�5熮 %�)*|1������xh(�v�P���a�mޗy|�DHZ�+��zΜ�[��!$�����!�-��6)�I��.�Z�.�R:=�S:�n��)#�<�ڮ��K���v#"�R)      &   1   x�3�4600�2�4Q��f ��3�P��� Js��(#Ns���� mp
G     