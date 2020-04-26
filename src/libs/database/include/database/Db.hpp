/*
 * Copyright (C) 2019 Emeric Poupon
 *
 * This file is part of LMS.
 *
 * LMS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LMS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LMS.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <filesystem>
#include <shared_mutex>

#include <Wt/Dbo/SqlConnectionPool.h>

namespace Database {

// Session living class handling the database and the login
class Db
{
	public:

		Db(const std::filesystem::path& dbPath);

	private:
		friend class Session;

		std::shared_mutex&		getMutex() { return _sharedMutex; }
		Wt::Dbo::SqlConnectionPool&	getConnectionPool() { return *_connectionPool; }

		class ScopedNoForeignKeys
		{
			public:
				ScopedNoForeignKeys(Db& db) : _db {db}
				{
					_db.executeSql("PRAGMA foreign_keys=OFF");
				}
				~ScopedNoForeignKeys()
				{
					_db.executeSql("PRAGMA foreign_keys=ON");
				}

			private:
			Db& _db;

		};

		void executeSql(const std::string& sql);

		std::shared_mutex				_sharedMutex;
		std::unique_ptr<Wt::Dbo::SqlConnectionPool>	_connectionPool;
};

} // namespace Database

