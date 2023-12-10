import decompress from "decompress";
import fs from "node:fs";
import path from "node:path";
import process from "process";
import util from 'node:util';
import { exec } from 'node:child_process';

const execp = util.promisify(exec);

const zipDir = process.argv[2];

(async () => {
    const zipFiles = (await fs.promises.readdir(zipDir))
        .filter(n => /^v0\.[0-9]+/.test(n))
        .sort((n1, n2) => n1.localeCompare(n2))
        .map(n => path.join(zipDir, n));

    let lastFiles = [];
    for (const zipFile of zipFiles) {
        for (const file of lastFiles) {
            file.type === "file" && (await fs.promises.rm(path.join("..", file.path)));
        }

        console.log(zipFile);

        const toSkipExt = { ".aps": true, ".ncb": true, ".opt": true, ".plg": true };
        const toSkipNames = { "alfatesting.txt": true, "alfatesting2.txt": true };
        const files = await decompress(zipFile, "..", {
            filter: file => !toSkipExt[path.extname(file.path)] && !toSkipNames[path.basename(file.path)]
        });
        const newerTs = new Date(Math.max(...files.map(file => file.mtime.getTime())));

        process.env["GIT_AUTHOR_DATE"] = newerTs;
        process.env["GIT_COMMITTER_DATE"] = newerTs;

        await execp("git add .", { cwd: ".." });
        await execp(`git commit -m "Unzipped ${path.basename(zipFile)}"`, { cwd: ".." });
        await execp(`git tag ${path.basename(zipFile).replace(".zip", "")}"`, { cwd: ".." });

        lastFiles = files;
    }
})();
