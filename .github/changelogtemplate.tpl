{{#versions}}
{{{label}}}

{{#sections}}
## {{{label}}}

{{#commits}}
* {{{subject}}} - {{commit.sha1_short}}  [{{{author}}}]
{{#body}}

{{{body_indented}}}
{{/body}}

{{/commits}}
{{/sections}}

{{/versions}}
