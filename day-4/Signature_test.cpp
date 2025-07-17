#include <test/jtx.h>
#include <xrpl/beast/unit_test.h>
#include <xrpld/app/rdb/backend/SQLiteDatabase.h>


namespace ripple {
namespace test {

// cmake --build . --target rippled --parallel 10 && ./rippled -u ripple.bootcamp.MyTests

struct MyTests_test : public beast::unit_test::suite
{
    /* signature test */
    void
    testSignatures(FeatureBitset features)
    {
        testcase("Signatures");

        using namespace jtx;

        Env env{*this, features};
        Account const alice{"alice"};
        Account const bob{"bob"};
        Account const carol{"carol"};
        env.fund(XRP(5000), alice, bob, carol);
        env.close();

        std::cout << "START" << std::endl;

        // Bad Permission (tefBAD_AUTH)
        // Sign the transaction with a key that does not match the account.
        std::cout << " ===== Test Bad Permission (tefBAD_AUTH) =====" << std::endl;
        env(pay(alice, bob, XRP(1)), sig(bob), ter(tefBAD_AUTH));
        env.close();

        // // Bad Permission (Master Disabled)
        // // Disable the master key and try to pay.
        // std::cout << " ===== Test Bad Permission (Master Disabled) =====" << std::endl;
        // env(regkey(alice, bob));
        // env(fset(alice, asfDisableMaster), sig(alice));
        // env(pay(alice, bob, XRP(1)), sig(alice), ter(tefMASTER_DISABLED));
        // env.close();

        // Bad Permission (Wrong RegularKey)
        // Set a regular key for carol to bob and try to sign with alice's key.
        std::cout << " ===== Test Bad Permission (Wrong RegularKey) =====" << std::endl;
        env(regkey(carol, bob));
        env(noop(carol), sig(alice), ter(tefBAD_AUTH));
        env(noop(carol), sig(bob), ter(tesSUCCESS));
        env.close();

        // Bad Signature (invalidTransaction)
        // Sign the transaction with a key that does not match the account.
        // Hint: Search the tests for "// Flip some bits in the signature."
        std::cout << " ===== Test Bad Signature (tefBAD_SIGNATURE) =====" << std::endl;
        auto badSig = env.jt(noop(alice)).stx->getFieldVL(sfTxnSignature);
        badSig[20] ^= 0xAA;
        env.jt(noop(alice), sig(alice), ter(tefBAD_SIGNATURE));
        env.close();

        // test alice's master key disabled, but carol's regular key can still sign
        std::cout << " ===== Test (Self) =====" << std::endl;
        env(regkey(alice, carol));
        env(pay(alice, bob, XRP(1)), sig(carol), ter(tesSUCCESS));
        env(fset(alice, asfDisableMaster), sig(alice)); // disable alice master key
        env(fset(carol, asfDisableMaster), sig(carol)); // disable carol master key
        env(pay(alice, bob, XRP(1)), sig(carol), ter(tesSUCCESS));
        env.close();

    }

    void run() override
    {
        using namespace test::jtx;
        auto const sa = supported_amendments();
        testSignatures(sa);
    }
};

BEAST_DEFINE_TESTSUITE(MyTests, bootcamp, ripple);      

}
}