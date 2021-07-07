/**
 * @file    Transaction.cpp
 * @ingroup SQLiteCpp
 * @brief   A Transaction is way to group multiple SQL statements into an atomic secured operation.
 *
 * Copyright (c) 2012-2019 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#include <SQLiteCpp/Transaction.h>

#include <SQLiteCpp/Database.h>
#include <SQLiteCpp/Assertion.h>


namespace SQLite
{


// Begins the SQLite transaction
Transaction::Transaction(Database& aDatabase) :
    mDatabase(&aDatabase)
{
    mDatabase->exec("BEGIN");
}

Transaction::Transaction(Transaction&& other)
    : mDatabase(other.mDatabase)
{
    other.mDatabase = nullptr;
}

// Safely rollback the transaction if it has not been committed.
Transaction::~Transaction()
{
    if (mDatabase != nullptr)
    {
        try
        {
            mDatabase->exec("ROLLBACK");
        }
        catch (SQLite::Exception&)
        {
            // Never throw an exception in a destructor: error if already rollbacked, but no harm is caused by this.
        }
    }
}

// Commit the transaction.
void Transaction::commit()
{
    if (mDatabase != nullptr)
    {
        mDatabase->exec("COMMIT");
        mDatabase = nullptr;
    }
    else
    {
        throw SQLite::Exception("Transaction already committed.");
    }
}


}  // namespace SQLite
