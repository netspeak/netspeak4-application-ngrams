import netspeak4py

parser = netspeak4py.QueryParser("", "", True)

options = netspeak4py.QueryParserOptions()
options.maxNormQueries = 1000
options.minLength = 1
options.maxLength = 5

q1 = parser.parse("Foo +", options)
print(q1)
assert len(q1) == 4
assert str(q1[0]) == '<netspeak4py.NormQuery "foo ? ? ? ?">'
assert str(q1[1]) == '<netspeak4py.NormQuery "foo ? ? ?">'
assert str(q1[2]) == '<netspeak4py.NormQuery "foo ? ?">'
assert str(q1[3]) == '<netspeak4py.NormQuery "foo ?">'
