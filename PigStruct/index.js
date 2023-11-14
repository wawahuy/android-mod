import fs from 'fs';
import StreamObject from 'stream-json/streamers/StreamObject.js';

// const source = fs.readFileSync('./il2cpp.h').toString('utf-8');
const source = fs.readFileSync('./test.h').toString('utf-8');

function find(root, name, tagUsed) {
    return root.inner.filter((inner) => {
        return inner.name == name && inner.tagUsed == tagUsed;
    })
}

function getSource(root) {
    const range = root.range;
    const begin = range.begin;
    const end = range.end;
    return source.substring(begin.offset, end.offset + 1);
}

function buildSource(root, name, tagUsed) {
    const stack = [
        { name, tagUsed, root }
    ];
    const sourceOutput = [];
    const sourceIds = {};
    while (stack.length) {
        const params = stack.pop();
        const structResults = find(params.root, params.name, params.tagUsed);
        structResults.forEach(struct => {
            if (struct.bases) {
                struct.bases.forEach((base) => {
                    const nameFieldType = base.type.qualType;
                    stack.push({ name: nameFieldType, tagUsed: "struct", root });
                })
            }

            (struct.inner || []).forEach((fields) => {
                if (fields.kind == "FieldDecl") {
                    let nameFieldType = fields.type.qualType;
                    if (nameFieldType.match(/^struct\s[a-zA-Z0-9_]$/)) {
                        nameFieldType = fields.type.desugaredQualType;
                    }
                    if (nameFieldType.match(/^struct\s[a-zA-Z0-9_](\s?)\*$/)) {
                        const a = /^struct\s(?<abc>[a-zA-Z0-9_])(\s?)\*$/.exec(nameFieldType);
                        nameFieldType = a.groups['abc'];
                    }
                    if (nameFieldType.match(/^[a-zA-Z0-9_](\s?)\*$/)) {
                        const a = /^(?<abc>[a-zA-Z0-9_])(\s?)\*$/.exec(nameFieldType);
                        nameFieldType = a.groups['abc'];
                    }
                    stack.push({ name: nameFieldType, tagUsed: "struct", root });
                }
            })
            if (!sourceIds[struct.id]) {
                sourceOutput.push(struct);
                sourceIds[struct.id] = true;
            }
        });
    }
    sourceOutput.reverse().forEach((source) => {
        console.log(getSource(source));
    })
}

(async () => {
    const jsonStream = StreamObject.withParser();
    let data = {};

    jsonStream.on('data', ({key, value}) => {
        data[key] = value;
        console.log('Push', key);
    });
    
    jsonStream.on('end', () => {
        buildSource(data, "UserData_o", "struct")
        // buildSource(data, "A", "struct")
    });

    fs.createReadStream('./output.json').pipe(jsonStream.input);
})();

