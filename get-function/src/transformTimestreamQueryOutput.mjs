export const transformTimestreamQueryOutput = (output) => {
  const { ColumnInfo, Rows } = output;

  return Rows.map(({ Data }) => {
    const object = {};

    Data.forEach(({ ScalarValue }, index) => {
      const columnName = ColumnInfo[index].Name;

      if (columnName === 'measure_name') {
        const measureValueColumnIndex = ColumnInfo.findIndex(({ Name }) => Name.startsWith('measure_value'));
        const measureValueColumn = ColumnInfo[measureValueColumnIndex];

        if (!measureValueColumn) {
          throw new Error('Measure value column missing!');
        }

        const measureValueScalarType = measureValueColumn.Type.ScalarType;
        const measureValueScalarValue = Data[measureValueColumnIndex].ScalarValue;

        switch (measureValueScalarType) {
          case 'DOUBLE':
            object[ScalarValue] = parseFloat(measureValueScalarValue);
            break;
          default:
            throw new Error(`Unsupported measure value scalar type "${measureValueScalarType}" for measure "${ScalarValue}"!`);
            break;
        }
      } else if (columnName === 'time') {
        object[columnName] = new Date(ScalarValue);
      } else if (!columnName.startsWith('measure_value')) {
        object[columnName] = ScalarValue;
      }
    });

    return object;
  });
};
