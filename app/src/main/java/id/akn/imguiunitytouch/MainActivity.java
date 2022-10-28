package id.akn.imguiunitytouch;

import android.app.Activity;
import android.os.Bundle;
import android.widget.Toast;
import java.util.Objects;

public class MainActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        if (!Objects.equals(this.getPackageName(), "id.akn.imguiunitytouch")) {
            System.loadLibrary("ModMenu");
        }

        Toast.makeText(this, "ImGui Unity Touch by Askan", Toast.LENGTH_LONG).show();
    }
}