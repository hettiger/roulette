import * as fs from "fs-extra";
import parse = require("csv-parse");
import ProgressBar = require("progress");
import exec from 'async-exec';
import { Client, BulkIndexDocumentsParams } from "elasticsearch";

const csvFilePath = __dirname + '/../data.csv';

const client = new Client({
    host: 'localhost:9200',
    keepAlive: true,
    maxRetries: 3,
    requestTimeout: 5000,
});

(async () => {
    const total = +await exec('wc -l < ' + csvFilePath) - 1;

    const bar = new ProgressBar(
        'Indiziere :current/:total [:bar] :percent, :rate/s, :etas verbleibend; ',
        {
            total,
            width: 20
        }
    );

    const params: BulkIndexDocumentsParams = {
        body: []
    };

    const stream = fs.createReadStream(csvFilePath, { start: 73 })
        .pipe(parse())
        .on('data', (csvrow) => {
            params.body.push(
                {
                    index: {
                        _index: 'roulette_v1',
                        _type: 'sample',
                    }
                },
                {
                    runden: +csvrow[0],
                    schwarz: +csvrow[1],
                    null: +csvrow[2],
                    rot: +csvrow[3],
                    bankroll: +csvrow[4],
                    naechster_einsatz: +csvrow[5],
                    maximale_bankroll: +csvrow[6]
                }
            );

            if (params.body.length % 5000 === 0) {
                stream.pause();
                client.bulk(params, stream.resume.bind(stream));
                params.body = [];
            }

            bar.tick();
        })
        .on('end', () => {
            if (params.body.length) {
                client.bulk(params);
                params.body = [];
            }
        });
})();
