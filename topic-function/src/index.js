const { TimestreamWriteClient, WriteRecordsCommand } = require("@aws-sdk/client-timestream-write");

const databaseName = process.env.DATABASE_NAME;
const tableName = process.env.TABLE_NAME;

const client = new TimestreamWriteClient();

exports.handler = async (event) => {
    console.log(JSON.stringify(event));

    const { clientId, humidity, sessionId, temperature, time } = event;
    const params = {
        DatabaseName: databaseName,
        Records: [
            {
                Dimensions: [
                    {
                        Name: 'clientId',
                        Value: clientId
                    },
                    {
                        Name: 'sessionId',
                        Value: sessionId
                    }
                ],
                MeasureName: 'sensors',
                MeasureValues: [
                    {
                        Name: 'humidity',
                        Value: humidity.toString(),
                        Type: 'DOUBLE'
                    },
                    {
                        Name: 'temperature',
                        Value: temperature.toString(),
                        Type: 'DOUBLE'
                    }
                ],
                MeasureValueType: 'MULTI',
                Time: new Date(time).getTime().toString(),
            }
        ],
        TableName: tableName,
    };

    console.log(JSON.stringify(params));

    const command = new WriteRecordsCommand(params);
    const response = await client.send(command);

    console.log(JSON.stringify(response));
};
