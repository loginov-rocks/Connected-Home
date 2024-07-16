import { TimestreamQueryClient, QueryCommand } from '@aws-sdk/client-timestream-query';

import { CLIENT_ID, TIMESTREAM_DATABASE_NAME, TIMESTREAM_TABLE_NAME } from './Constants.mjs';
import { transformTimestreamQueryOutput } from './transformTimestreamQueryOutput.mjs';

const timestreamQueryClient = new TimestreamQueryClient();

export const handler = async (event) => {
  console.log('event', JSON.stringify(event));

  const sqlQueryStart = `SELECT * FROM "${TIMESTREAM_DATABASE_NAME}"."${TIMESTREAM_TABLE_NAME}" WHERE "clientId" = '${CLIENT_ID}' AND "measure_name" = '`;
  const sqlQueryEnd = '\' ORDER BY "time" DESC LIMIT 1';

  const queryHumidityCommand = new QueryCommand({
    QueryString: sqlQueryStart + 'humidity' + sqlQueryEnd,
  });

  const queryTemperatureCommand = new QueryCommand({
    QueryString: sqlQueryStart + 'temperature' + sqlQueryEnd,
  });

  let queryHumidityOutput, queryTemperatureOutput;
  try {
    [queryHumidityOutput, queryTemperatureOutput] = await Promise.all([
      timestreamQueryClient.send(queryHumidityCommand),
      timestreamQueryClient.send(queryTemperatureCommand),
    ]);
  } catch (error) {
    console.error(error);

    return { statusCode: 500 };
  }

  console.log('queryHumidityOutput', JSON.stringify(queryHumidityOutput));
  console.log('queryTemperatureOutput', JSON.stringify(queryTemperatureOutput));

  const humidityRows = transformTimestreamQueryOutput(queryHumidityOutput);
  const temperatureRows = transformTimestreamQueryOutput(queryTemperatureOutput);

  return {
    body: JSON.stringify({
      humidity: {
        humidity: humidityRows[0].humidity,
        time: humidityRows[0].time,
      },
      temperature: {
        temperature: temperatureRows[0].temperature,
        time: temperatureRows[0].time,
      },
    }),
    headers: {
      'content-type': 'application/json',
    },
    statusCode: 200,
  };
};
