import { TimestreamQueryClient, QueryCommand } from '@aws-sdk/client-timestream-query';

export const handler = async (event) => {
    const client = new TimestreamQueryClient();

    const command = new QueryCommand({
        QueryString: 'SELECT * FROM myTestDb.myTestTable WHERE measure_name = \'temperature\' ORDER BY time DESC limit 1'
    });

    const response = await client.send(command);

    return response;
};
