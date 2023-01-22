const { TimestreamQueryClient, QueryCommand } = require('@aws-sdk/client-timestream-query');

const databaseName = process.env.DATABASE_NAME;
const tableName = process.env.TABLE_NAME;

const client = new TimestreamQueryClient();

exports.handler = async (event) => {
    console.log(JSON.stringify(event));

    const params = {
        QueryString: `SELECT * FROM "${databaseName}"."${tableName}" WHERE time > ago(1d) ORDER BY time DESC LIMIT 1`
    };

    console.log(JSON.stringify(params));

    const command = new QueryCommand(params);
    const response = await client.send(command);

    console.log(JSON.stringify(response));

    return response;
};
