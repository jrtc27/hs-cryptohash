import Test.HUnit
import Data.Char
import Data.Bits
import Data.Word
import qualified Data.ByteString as B
import qualified Data.CryptoHash.MD2 as MD2
import qualified Data.CryptoHash.MD4 as MD4
import qualified Data.CryptoHash.MD5 as MD5
import qualified Data.CryptoHash.SHA1 as SHA1
import qualified Data.CryptoHash.SHA224 as SHA224
import qualified Data.CryptoHash.SHA256 as SHA256
import qualified Data.CryptoHash.SHA384 as SHA384
import qualified Data.CryptoHash.SHA512 as SHA512
import qualified Data.CryptoHash.RIPEMD160 as RIPEMD160

v0 = ""
v1 = "The quick brown fox jumps over the lazy dog"
v2 = "The quick brown fox jumps over the lazy cog"
vectors = [ v0, v1, v2 ]

results :: [ (String, (B.ByteString -> B.ByteString), [String]) ]
results = [
	("MD2", MD2.hash, [
		"8350e5a3e24c153df2275c9f80692773",
		"03d85a0d629d2c442e987525319fc471",
		"6b890c9292668cdbbfda00a4ebf31f05" ]),
	("MD4", MD4.hash, [
		"31d6cfe0d16ae931b73c59d7e0c089c0",
		"1bee69a46ba811185c194762abaeae90",
		"b86e130ce7028da59e672d56ad0113df" ]),
	("MD5", MD5.hash, [
		"d41d8cd98f00b204e9800998ecf8427e",
		"9e107d9d372bb6826bd81d3542a419d6",
		"1055d3e698d289f2af8663725127bd4b" ]),
	("SHA1", SHA1.hash, [
		"da39a3ee5e6b4b0d3255bfef95601890afd80709",
		"2fd4e1c67a2d28fced849ee1bb76e7391b93eb12",
		"de9f2c7fd25e1b3afad3e85a0bd17d9b100db4b3" ]),
	("SHA224", SHA224.hash, [
		"d14a028c2a3a2bc9476102bb288234c415a2b01f828ea62ac5b3e42f",
		"730e109bd7a8a32b1cb9d9a09aa2325d2430587ddbc0c38bad911525",
		"fee755f44a55f20fb3362cdc3c493615b3cb574ed95ce610ee5b1e9b" ]),
	("SHA256", SHA256.hash, [
		"e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855",
		"d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592",
		"e4c4d8f3bf76b692de791a173e05321150f7a345b46484fe427f6acc7ecc81be" ]),
	("SHA384", SHA384.hash, [
		"38b060a751ac96384cd9327eb1b1e36a21fdb71114be07434c0cc7bf63f6e1da274edebfe76f65fbd51ad2f14898b95b",
		"ca737f1014a48f4c0b6dd43cb177b0afd9e5169367544c494011e3317dbf9a509cb1e5dc1e85a941bbee3d7f2afbc9b1",
		"098cea620b0978caa5f0befba6ddcf22764bea977e1c70b3483edfdf1de25f4b40d6cea3cadf00f809d422feb1f0161b" ]),
	("SHA512", SHA512.hash, [
		"cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e",
		"07e547d9586f6a73f73fbac0435ed76951218fb7d0c8d788a309d785436bbb642e93a252a954f23912547d1e8a3b5ed6e1bfd7097821233fa0538f3db854fee6",
		"3eeee1d0e11733ef152a6c29503b3ae20c4f1f3cda4cb26f1bc1a41f91c7fe4ab3bd86494049e201c4bd5155f31ecb7a3c8606843c4cc8dfcab7da11c8ae5045" ]),
	("RIPEMD160", RIPEMD160.hash, [
		"b00353262e04ff3abc71e6da8732513837eb0a0f",
		"83a67764e2c2ddb90cc0ec49efac7030472bb554", 
		"08b478fe69a35c44fb2cd423ae1b8bbfa883db61" ])
	]

hexalise s =
        concatMap (\c -> [ hex $ c `div` 16, hex $ c `mod` 16 ]) s
        where hex i
                | i >= 0 && i <= 9   = fromIntegral (ord '0') + i
                | i >= 10 && i <= 15 = fromIntegral (ord 'a') + i - 10
                | otherwise          = 0

hexaliseB :: B.ByteString -> B.ByteString
hexaliseB = B.pack . hexalise . B.unpack

runhash :: (B.ByteString -> B.ByteString) -> String -> String
runhash hash v = map (toEnum.fromEnum) $ hexalise $ B.unpack $ hash $ B.pack $ map (toEnum.fromEnum) v

--mapAlgIter
--mapAlg (n, h, r) = map (\(ri, i) -> TestLabel (n ++ " " ++ show i) mapAlgTest (TestCase (assertEqual ))) $ zip r [0..]
--mapTests = map mapAlg results 
makeTestAlg (name, hash, results) = map maketest $ zip3 [0..] vectors results
	where
		testname i = name ++ " " ++ show i
		maketest (i, v, r) = testname i ~: testname i ~: r ~=? (runtest v)
			--TestCase $ do assertString (name ++ " " ++ show i) r (runtest v)
		runtest v = runhash hash v

mapTests :: [Test]
mapTests = concatMap makeTestAlg results

tests = TestList mapTests

main = runTestTT tests